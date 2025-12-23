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

#define LUA_CLASS(name) \
	(s_luaClasses.push_back({#name}), #name)
#define LAST_LUA_CLASS s_luaClasses[s_luaClasses.size() - 1]
// Declares a field for the LAST_LUA_CLASS.
#define LUA_FIELD(name, type) \
	(LAST_LUA_CLASS.fields.push_back({#name, #type}), #name)
// Declares a class base for the LAST_LUA_CLASS.
#define LUA_CLASS_BASE(name) \
	(LAST_LUA_CLASS.base = #name, sol::bases<name>())


namespace Tank
{
	void UserTypes::allGLM(sol::state &lua)
	{
		auto utVec3 = lua.new_usertype<glm::vec3>(
			LUA_CLASS(Vec3),
			sol::constructors<glm::vec3(), glm::vec3(float, float, float)>()
		);
		utVec3[LUA_FIELD(x, number)] = &glm::vec3::x;
		utVec3[LUA_FIELD(y, number)] = &glm::vec3::y;
		utVec3[LUA_FIELD(z, number)] = &glm::vec3::z;
	}


	void UserTypes::allNodes(sol::state &lua)
	{
		// Define types Node is dependent on

		// Transform
		auto utTransform = lua.new_usertype<Transform>(
			LUA_CLASS(Transform)
		);
		utTransform[LUA_FIELD(translation, Vec3)] = sol::property(&Transform::getLocalTranslation, &Transform::setLocalTranslation);
		utTransform[LUA_FIELD(rotation, Vec3)] = sol::property(&Transform::getLocalRotation, &Transform::setLocalRotation);

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
		utNode[LUA_FIELD(name, string)] = sol::property(&Node::getName, &Node::setName);
		utNode[LUA_FIELD(transform, Transform)] = sol::property(&Node::getTransform);
		utNode[LUA_FIELD(key_input, KeyInput)] = sol::property(&Node::getKeyInput);

		sol::usertype<Camera> utCamera = lua.new_usertype<Camera>(
			LUA_CLASS(Camera),
			sol::base_classes, LUA_CLASS_BASE(Node)
		);
		utCamera["set_pos"] = &Camera::setPosition;
	}


	void UserTypes::all(sol::state &lua)
	{
		UserTypes::allGLM(lua);
		UserTypes::allNodes(lua);
	}


	void UserTypes::codegen()
	{
		Res codegenPath = Res("TankLuaDocs/autogen.lua", true);
		std::string firstLine = "---@meta (GENERATED)\n";

		std::ofstream stream;
		// @todo may throw
		stream.open(codegenPath.resolvePathStr(), std::ofstream::out | std::ofstream::trunc);
		//if (!File::writeLines(codegenPath.resolvePath(), firstLine))
		//{
		//	TE_CORE_WARN(std::format("Codegen > Failed to clear {}. Cancelling...", codegenPath.resolvePathStr()));
		//	return;
		//}

		stream << firstLine;
		for (const LuaClass &lc : s_luaClasses)
		{
			stream << lc << "\n";
		}

		s_luaClasses.clear();
	}
}