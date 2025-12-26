#include <GLFW/glfw3.h>
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


#define KC_PAIR(x) #x, GLFW_KEY_##x

// Declare a Sol class usertype.
#define LUA_CLASS(name) (s_luaClasses.push_back({#name}), #name)
// Get a lua class from name
#define GET_LUA_CLASS(name) Tank::UserTypes::classFromName(#name).value()

// Declares a class base for name.
#define LUA_CLASS_BASE(name, baseName) (GET_LUA_CLASS(name)->base = #baseName/*, sol::bases<baseName>()*/)

// Declares a class field.
#define LUA_FIELD(className, name, type) \
	(GET_LUA_CLASS(className)->fields.push_back({#name, #type}), #name)

#define LUA_FUNCTION(className, funcName, retType, ...) \
	GET_LUA_CLASS(className)->functions.push_back({ \
		#funcName, \
		{ ##__VA_ARGS__ }, \
		#retType \
	})
#define LUA_CONSTRUCTOR(className, ...) \
	LUA_FUNCTION(className, new, className, ##__VA_ARGS__)


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


	void UserTypes::allGLM(sol::state &lua)
	{
		auto utVec3 = lua.new_usertype<glm::vec3>(
			LUA_CLASS(Vec3),
			sol::constructors<glm::vec3(), glm::vec3(float, float, float)>()
		);
		LUA_CONSTRUCTOR(Vec3);
		LUA_CONSTRUCTOR(Vec3, {{"x", "number"}, {"y", "number"}, {"z", "number"}});

		utVec3[LUA_FIELD(Vec3, x, number)] = &glm::vec3::x;
		utVec3[LUA_FIELD(Vec3, y, number)] = &glm::vec3::y;
		utVec3[LUA_FIELD(Vec3, z, number)] = &glm::vec3::z;
	}


	void UserTypes::allNodes(sol::state &lua)
	{
		// Define types Node is dependent on

		// Transform
		auto utTransform = lua.new_usertype<Transform>(
			LUA_CLASS(Transform)
		);
		utTransform[LUA_FIELD(Transform, translation, Vec3)] = sol::property(&Transform::getLocalTranslation, &Transform::setLocalTranslation);
		utTransform[LUA_FIELD(Transform, rotation, Vec3)] = sol::property(&Transform::getLocalRotation, &Transform::setLocalRotation);
		utTransform[LUA_FIELD(Transform, scale, Vec3)] = sol::property(&Transform::getLocalScale, &Transform::setLocalScale);
		
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

		sol::usertype<KeyInput> utKeyInput = lua.new_usertype<KeyInput>(
			LUA_CLASS(KeyInput)
		);
		utKeyInput["get_key_state"] = &KeyInput::getKeyState;

		// Now we can define Node and its subclasses
		sol::usertype<Node> utNode = lua.new_usertype<Node>(
			LUA_CLASS(Node)
		);
		utNode[LUA_FIELD(Node, name, string)] = sol::property(&Node::getName, &Node::setName);
		utNode[LUA_FIELD(Node, transform, Transform)] = sol::property(&Node::getTransform);
		utNode[LUA_FIELD(Node, key_input, KeyInput)] = sol::property(&Node::getKeyInput);

		sol::usertype<Camera> utCamera = lua.new_usertype<Camera>(
			LUA_CLASS(Camera),
			sol::base_classes, sol::bases<Node>()
		);
		LUA_CLASS_BASE(Camera, Node);
		utCamera["set_pos"] = &Camera::setPosition;
	}


	void UserTypes::all(sol::state &lua)
	{
		UserTypes::allGLM(lua);
		UserTypes::allNodes(lua);
	}


	void UserTypes::codegen()
	{
		//if (!File::writeLines(codegenPath.resolvePath(), firstLine))
		//{
		//	TE_CORE_WARN(std::format("Codegen > Failed to clear {}. Cancelling...", codegenPath.resolvePathStr()));
		//	return;
		//}

		for (const LuaClass &lc : s_luaClasses)
		{
			TE_CORE_INFO(lc.base);
			Res codegenPath = Res(std::format("TankLuaDocs/{}.lua", lc.name), true);

			std::ofstream stream;
			// @todo may throw
			stream.open(codegenPath.resolvePathStr(), std::ofstream::out | std::ofstream::trunc);

			stream << "---@meta (GENERATED)\n";
			stream << lc << "\n";
		}

		s_luaClasses.clear();
	}
}