#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <log.hpp>
#include <fs/file.hpp>
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
#include <static/time.hpp>


#define KC(x) #x, GLFW_KEY_##x
#define PAIR(x, y) x, y

// Declare a Sol class usertype.	
#define SOL_CLASS(name) (s_luaClasses.push_back({#name}), #name)
// Get a lua class from name
#define GET_SOL_CLASS(name) Tank::UserTypes::findItem<LuaClass>(s_luaClasses, name)
// Declares a class base for name.
#define SOL_CLASS_BASE(name, baseName) (GET_SOL_CLASS(name)->base = #baseName/*, sol::bases<baseName>()*/)

// Declares a class field.
#define SOL_FIELD(className, name, type) \
	(GET_SOL_CLASS(className)->fields.push_back({name, type}), name)
#define SOL_STATIC_FIELD(className, name, type) \
	(GET_SOL_CLASS(className)->staticFields.push_back({name, type}), name)
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

// Define an enum
#define SOL_ENUM(name) (s_luaEnums.push_back({#name}), #name)
#define GET_SOL_ENUM(name) Tank::UserTypes::findItem<LuaEnum>(s_luaEnums, name)
#define SOL_ENUM_PAIR(enumName, pair) GET_SOL_ENUM(enumName)->pairs.push_back({pair})


namespace Tank
{
	template <Named T>
	T *UserTypes::findItem(std::vector<T> &items, const std::string &name)
	{
		// First UserType class whose name matches the key
		auto it = std::find_if(items.begin(), items.end(), [&name](const T &cls) { return name == cls.name; });
		if (it == items.end())
		{
			TE_CORE_ERROR(std::format("Codegen > Tried to find item with name {}, but it didn't exist.", name));
			return {};
		}

		return &(*it);
	}


	template<>
	void UserTypes::generate<glm::vec3>(sol::state &lua)
	{
		// Vec3
		auto ut = lua.new_usertype<glm::vec3>(
			SOL_CLASS(Vec3),
			sol::constructors<glm::vec3(), glm::vec3(float, float, float)>()
		);
		SOL_CONSTRUCTOR("Vec3", {});
		SOL_CONSTRUCTOR("Vec3", { {"x", "number"}, {"y", "number"}, {"z", "number"} });

		// Fields
		ut[SOL_FIELD("Vec3", "x", "number")] = &glm::vec3::x;
		ut[SOL_FIELD("Vec3", "y", "number")] = &glm::vec3::y;
		ut[SOL_FIELD("Vec3", "z", "number")] = &glm::vec3::z;

		// Meta functions
		ut[SOL_META_METHOD(
			"Vec3",
			sol::meta_function::addition,
			"Vec3",
			{ { "other", "Vec3" } }
		)] = [](const glm::vec3 &a, const glm::vec3 &b) { return a + b; };
		ut[SOL_META_METHOD(
			"Vec3",
			sol::meta_function::subtraction,
			"Vec3",
			{ { "other", "Vec3" } }
		)] = [](const glm::vec3 &a, const glm::vec3 &b) { return a - b; };
		ut[SOL_META_METHOD(
			"Vec3",
			sol::meta_function::multiplication,
			"Vec3",
			{ { "scalar", "number" } }
		)] = [](const glm::vec3 &a, const float b) { return a * b; };
		ut[SOL_META_METHOD(
			"Vec3",
			sol::meta_function::division,
			"Vec3",
			{ { "scalar", "number" } }
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
			{ { "other", "Vec3" } }
		);
		SOL_META_METHOD(
			"Vec3",
			sol::meta_function::to_string,
			"string",
			{}
		);
	}

	template<>
	void UserTypes::generate<KeyState>(sol::state &lua)
	{
		// New KeyCode type to represent GLFW_KEY_ values
		lua.new_enum(
			SOL_ENUM(KeyCode),
			KC(W),
			KC(A),
			KC(S),
			KC(D)
		);
		SOL_ENUM_PAIR("KeyCode", KC(W));
		SOL_ENUM_PAIR("KeyCode", KC(A));
		SOL_ENUM_PAIR("KeyCode", KC(S));
		SOL_ENUM_PAIR("KeyCode", KC(D));

		lua.new_enum(
			SOL_ENUM(KeyState),
			PAIR("Held", (int)KeyState::Held),
			PAIR("Pressed", (int)KeyState::Pressed),
			PAIR("NotPressed", (int)KeyState::NotPressed),
			PAIR("Released", (int)KeyState::Released)
		);
		SOL_ENUM_PAIR("KeyState", PAIR("Held", (int)KeyState::Held));
		SOL_ENUM_PAIR("KeyState", PAIR("Pressed", (int)KeyState::Pressed));
		SOL_ENUM_PAIR("KeyState", PAIR("NotPressed", (int)KeyState::NotPressed));
		SOL_ENUM_PAIR("KeyState", PAIR("Released", (int)KeyState::Released));
	}

	template<>
	void UserTypes::generate<KeyInput>(sol::state &lua)
	{
		sol::usertype<KeyInput> utKeyInput = lua.new_usertype<KeyInput>(
			SOL_CLASS(KeyInput)
		);
		utKeyInput[SOL_METHOD("KeyInput", "get_key_state", "KeyState", { { "code", "KeyCode" } })] = &KeyInput::getKeyState;
	}

	template<>
	void UserTypes::generate<Node>(sol::state &lua)
	{
		sol::usertype<Node> ut = lua.new_usertype<Node>(
			SOL_CLASS(Node)
		);
		ut[SOL_FIELD("Node", "name", "string")] = sol::property(&Node::getName, &Node::setName);
		ut[SOL_FIELD("Node", "transform", "Transform")] = sol::property(&Node::getTransform);
		ut[SOL_FIELD("Node", "key_input", "KeyInput")] = sol::property(&Node::getKeyInput);
		ut[SOL_METHOD("Node", "get_parent", "Node", {})] = &Node::getParent;
		ut[SOL_METHOD("Node", "get_child", "Node", { { "index", "number" } })] = static_cast<Node * (Node:: *)(int) const>(&Node::getChild);
		ut[SOL_METHOD("Node", "get_child", "Node", { { "name", "string" } })] = static_cast<Node * (Node:: *)(const std::string &) const>(&Node::getChild);
		SOL_GLOBAL_FIELD("Node", "node", "Node");
	}

	template<>
	void UserTypes::generate<Camera>(sol::state &lua)
	{
		sol::usertype<Camera> ut = lua.new_usertype<Camera>(
			SOL_CLASS(Camera),
			sol::base_classes, sol::bases<Node>()
		);
		SOL_CLASS_BASE("Camera", Node);
		ut[SOL_METHOD("Camera", "set_pos", "", { { "pos", "Vec3" } })] = &Camera::setPosition;
	}

	template<>
	void UserTypes::generate<Scene>(sol::state &lua)
	{
		// Scene
		sol::usertype<Scene> ut = lua.new_usertype<Scene>(
			SOL_CLASS(Scene),
			sol::base_classes, sol::bases<Node>()
		);
		SOL_CLASS_BASE("Scene", Node);
		ut[SOL_METHOD("Scene", "active_camera", "Camera", {})] = &Scene::getActiveCamera;
		ut[SOL_STATIC_METHOD("Scene", "current", "Scene", {})] = &Scene::getActiveScene;
	}

	template<>
	void UserTypes::generate<Transform>(sol::state &lua)
	{
		auto ut = lua.new_usertype<Transform>(
			SOL_CLASS(Transform)
		);
		ut[SOL_FIELD("Transform", "translation", "Vec3")] = sol::property(&Transform::getLocalTranslation, &Transform::setLocalTranslation);
		ut[SOL_FIELD("Transform", "rotation", "Vec3")] = sol::property(&Transform::getLocalRotation, &Transform::setLocalRotation);
		ut[SOL_FIELD("Transform", "scale", "Vec3")] = sol::property(&Transform::getLocalScale, &Transform::setLocalScale);
	}

	template<>
	void UserTypes::generate<Time>(sol::state &lua)
	{
		auto ut = lua.new_usertype<Time>(
			SOL_CLASS(Time)
		);
		ut[SOL_STATIC_FIELD("Time", "delta", "number")] = sol::property(&Time::getFrameDelta);
	}

	
	void UserTypes::generate(sol::state &lua)
	{
		generate<glm::vec3>(lua);
		
		generate<KeyState>(lua);
		generate<KeyInput>(lua);
		
		generate<Node>(lua);
		generate<Camera>(lua);
		generate<Scene>(lua);
		generate<Transform>(lua);

		generate<Time>(lua);
	}


	void UserTypes::codegen()
	{
		fs::path classes = fs::path{ "lua" } / "codegen" / "classes";
		fs::path enums = fs::path{ "lua" } / "codegen" / "enums";
		fs::create_directories(Res(classes, false).resolvePath());
		fs::create_directories(Res(enums, false).resolvePath());
		for (const LuaClass &lc : s_luaClasses)
		{
			Res codegenPath = Res(classes / (lc.name + ".lua"), false);
			generateFile(codegenPath, lc);
		}
		for (const LuaEnum &le : s_luaEnums)
		{
			Res codegenPath = Res(enums / (le.name + ".lua"), false);
			generateFile(codegenPath, le);
		}

		s_luaClasses.clear();
		s_luaEnums.clear();
	}


	template <StreamWritable T>
	void UserTypes::generateFile(const Res &res, const T &t)
	{
		TE_CORE_TRACE(std::format("Codegen > Generating file for type {}", t.name));
		std::ofstream stream;

		stream.open(res.resolvePathStr(), std::ofstream::out | std::ofstream::trunc);

		if (!stream)
		{
			TE_CORE_ERROR(std::format("Codegen > Error generating file for type {}", t.name));
			return;
		}

		stream << "---@meta (GENERATED)\n";
		stream << t << "\n";
		stream.close();
	}
}