#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <log.hpp>
#include <file.hpp>
#include <assets/resource.hpp>
#include "user_types.hpp"
#include "sol/sol.hpp"
#include "lua_codegen.hpp"
// The types to-be-defined as usertypes
#include <glm/glm.hpp>
#include <transform.hpp>
#include <key_input.hpp>
#include <nodes/node.hpp>
#include <nodes/camera.hpp>
#include <nodes/scene.hpp>


#define KC_PAIR(x) #x, GLFW_KEY_##x

// Declare a Sol class usertype.	
#define SOL_CLASS(name) (s_luaClasses.push_back({#name}), #name)
// Get a lua class from name
#define GET_SOL_CLASS(name) Tank::UserTypes::classFromName(name).value()
// Declares a class base for name.
#define SOL_CLASS_BASE(name, baseName) (GET_SOL_CLASS(name)->base = #baseName/*, sol::bases<baseName>()*/)

// Declares a class field.
#define SOL_FIELD(className, name, type) \
	(GET_SOL_CLASS(className)->fields.push_back({name, type}), name)
#define SOL_GLOBAL_FIELD(className, name, type) \
	GET_SOL_CLASS(className)->globalFields.push_back({name, type})

#define SOL_CALLABLE(relevantClass, functionsVector, funcName, retType, ...) \
	GET_SOL_CLASS(relevantClass)->functionsVector.push_back({ \
		funcName, \
		##__VA_ARGS__, \
		retType \
	})
#define SOL_METHOD(className, name, retType, ...) \
	(SOL_CALLABLE( \
		className, \
		methods, \
		name, \
		retType, \
		__VA_ARGS__ \
	), name)
#define SOL_META_METHOD(className, methodVal, retType, ...) \
	(SOL_CALLABLE( \
		className, \
		metaMethods, \
		sol::meta_function_names()[(int)methodVal], \
		retType, \
		__VA_ARGS__ \
	), methodVal)
#define SOL_STATIC_METHOD(className, name, retType, ...) \
	(SOL_CALLABLE( \
		className, \
		staticMethods, \
		name, \
		retType, \
		__VA_ARGS__ \
	), name)
#define SOL_GLOBAL_FUNCTION(relevantClass, name, retType, ...) \
	(SOL_CALLABLE( \
		relevantClass, \
		globalFunctions, \
		name, \
		retType, \
		__VA_ARGS__ \
	), name)
#define SOL_CONSTRUCTOR(className, ...) \
	SOL_STATIC_METHOD(className, "new", className, __VA_ARGS__)


namespace Tank
{
	std::optional<LuaClass*> UserTypes::classFromName(const std::string &name)
	{
		// First UserType class whose name matches the key
		auto it = std::find_if(s_luaClasses.begin(), s_luaClasses.end(), [&name](const LuaClass &cls) { return name == cls.name; });
		if (it == s_luaClasses.end())
		{
			TE_CORE_ERROR(std::format("Codegen > Tried to find class with name {}, but it didn't exist.", name));
			return {};
		}

		return &(*it);
	}


	void UserTypes::generate(sol::state &lua)
	{
		// Vec3
		auto utVec3 = lua.new_usertype<glm::vec3>(
			SOL_CLASS(Vec3),
			sol::constructors<glm::vec3(), glm::vec3(float, float, float)>()
		);
		SOL_CONSTRUCTOR("Vec3", {});
		SOL_CONSTRUCTOR("Vec3", {{"x", "number"}, {"y", "number"}, {"z", "number"}});

		// Fields
		utVec3[SOL_FIELD("Vec3", "x", "number")] = &glm::vec3::x;
		utVec3[SOL_FIELD("Vec3", "y", "number")] = &glm::vec3::y;
		utVec3[SOL_FIELD("Vec3", "z", "number")] = &glm::vec3::z;

		// Meta functions
		utVec3[SOL_META_METHOD(
			"Vec3",
			sol::meta_function::addition,
			"Vec3",
			{{ "other", "Vec3" }}
		)] = [](const glm::vec3 &a, const glm::vec3 &b) { return a + b; };
		utVec3[SOL_META_METHOD(
			"Vec3",
			sol::meta_function::subtraction,
			"Vec3",
			{{ "other", "Vec3" }}
		)] = [](const glm::vec3 &a, const glm::vec3 &b) { return a - b; };
		utVec3[SOL_META_METHOD(
			"Vec3",
			sol::meta_function::multiplication,
			"Vec3",
			{{ "scalar", "number" }}
		)] = [](const glm::vec3 &a, const float b) { return a * b; };
		utVec3[SOL_META_METHOD(
			"Vec3",
			sol::meta_function::division,
			"Vec3",
			{{ "scalar", "number" }}
		)] = [](const glm::vec3 &a, const float b) { return a / b; };
		SOL_META_METHOD(
			"Vec3",
			sol::meta_function::unary_minus,
			"Vec3",
			{}
		);
		SOL_META_METHOD(
			"Vec3",
			sol::meta_function::equal_to,
			"boolean",
			{{ "other", "Vec3" }}
		);
		SOL_META_METHOD(
			"Vec3",
			sol::meta_function::to_string,
			"string",
			{}
		);

		// Define types Node is dependent on
		// Transform
		auto utTransform = lua.new_usertype<Transform>(
			SOL_CLASS(Transform)
		);
		utTransform[SOL_FIELD("Transform", "translation", "Vec3")] = sol::property(&Transform::getLocalTranslation, &Transform::setLocalTranslation);
		utTransform[SOL_FIELD("Transform", "rotation", "Vec3")] = sol::property(&Transform::getLocalRotation, &Transform::setLocalRotation);
		utTransform[SOL_FIELD("Transform", "scale", "Vec3")] = sol::property(&Transform::getLocalScale, &Transform::setLocalScale);
		
		// KeyInput, and relevant enums
		lua.new_enum(
			"KeyState",
			"Held", KeyState::Held,
			"Pressed", KeyState::Pressed,
			"NotPressed", KeyState::NotPressed,
			"Released", KeyState::Released
		);
		lua.new_enum(
			"KeyCode",
			KC_PAIR(W),
			KC_PAIR(A),
			KC_PAIR(S),
			KC_PAIR(D)
		);


		// KeyInput
		sol::usertype<KeyInput> utKeyInput = lua.new_usertype<KeyInput>(
			SOL_CLASS(KeyInput)
		);
		utKeyInput[SOL_METHOD("KeyInput", "get_key_state", "KeyState", {{ "code", "KeyCode" }})] = &KeyInput::getKeyState;


		// Node
		sol::usertype<Node> utNode = lua.new_usertype<Node>(
			SOL_CLASS(Node)
		);
		utNode[SOL_FIELD("Node", "name", "string")] = sol::property(&Node::getName, &Node::setName);
		utNode[SOL_FIELD("Node", "transform", "Transform")] = sol::property(&Node::getTransform);
		utNode[SOL_FIELD("Node", "key_input", "KeyInput")] = sol::property(&Node::getKeyInput);
		utNode[SOL_METHOD("Node", "get_parent", "Node", {})] = &Node::getParent;
		utNode[SOL_METHOD("Node", "get_child", "Node", {{ "index", "number" }})] = static_cast<Node *(Node::*)(int) const>(&Node::getChild);
		utNode[SOL_METHOD("Node", "get_child", "Node", {{ "name", "string" }})] = static_cast<Node *(Node::*)(const std::string &) const>(&Node::getChild);
		SOL_GLOBAL_FIELD("Node", "node", "Node");


		// Camera
		sol::usertype<Camera> utCamera = lua.new_usertype<Camera>(
			SOL_CLASS(Camera),
			sol::base_classes, sol::bases<Node>()
		);
		SOL_CLASS_BASE("Camera", Node);
		utCamera[SOL_METHOD("Camera", "set_pos", "", {{ "pos", "Vec3" }})] = &Camera::setPosition;


		// Scene
		sol::usertype<Scene> utScene = lua.new_usertype<Scene>(
			SOL_CLASS(Scene),
			sol::base_classes, sol::bases<Node>()
		);
		SOL_CLASS_BASE("Scene", Node);
		utScene[SOL_METHOD("Scene", "active_camera", "Camera", {})] = &Scene::getActiveCamera;
		utScene[SOL_STATIC_METHOD("Scene", "current", "Scene", {})] = &Scene::getActiveScene;
	}


	void UserTypes::codegen()
	{
		//if (!File::writeLines(codegenPath.resolvePath(), firstLine))
		//{
		//	TE_CORE_WARN(std::format("Codegen > Failed to clear {}. Cancelling...", codegenPath.resolvePathStr()));
		//	return;
		//}

		fs::create_directories("TankLuaDocs/codegen");
		for (const LuaClass &lc : s_luaClasses)
		{
			Res codegenPath = Res(std::format("TankLuaDocs/codegen/{}.lua", lc.name), true);

			std::ofstream stream;
			// @todo may throw
			stream.open(codegenPath.resolvePathStr(), std::ofstream::out | std::ofstream::trunc);

			stream << "---@meta (GENERATED)\n";
			stream << lc << "\n";
		}

		s_luaClasses.clear();
	}
}