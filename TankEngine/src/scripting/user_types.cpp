#include "user_types.hpp"
#include "sol/sol.hpp"
#include <GLFW/glfw3.h>
// The types to-be-defined as usertypes
#include <glm/glm.hpp>
#include <nodes/node.hpp>
#include <transform.hpp>
#include <key_input.hpp>


#define KC_PAIR(x) #x, GLFW_KEY_##x


namespace Tank
{
	void UserTypes::allGLM(sol::state &lua)
	{
		sol::usertype<glm::vec3> vec3 = lua.new_usertype<glm::vec3>(
			"vec3",
			sol::constructors<glm::vec3(float, float, float)>()
		);
		vec3["x"] = &glm::vec3::x;
		vec3["y"] = &glm::vec3::y;
		vec3["z"] = &glm::vec3::z;
	}


	void UserTypes::allNodes(sol::state &lua)
	{
		// Define types Node is dependent on

		// Transform
		sol::usertype<Transform> transformType = lua.new_usertype<Transform>("Transform");
		transformType["translation"] = sol::property(&Transform::getLocalTranslation, &Transform::setLocalTranslation);
		transformType["rotation"] = sol::property(&Transform::getLocalRotation, &Transform::setLocalRotation);

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

		sol::usertype<KeyInput> kiType = lua.new_usertype<KeyInput>("KeyInput");
		kiType["get_key_state"] = &KeyInput::getKeyState;

		// Now we can define Node and its subclasses
		sol::usertype<Node> nodeType = lua.new_usertype<Node>("Node");
		nodeType["name"] = sol::property(&Node::getName, &Node::setName);
		nodeType["transform"] = sol::property(&Node::getTransform);
		nodeType["key_input"] = sol::property(&Node::getKeyInput);
		nodeType["destroy"] = &Node::destroy;
	}


	void UserTypes::all(sol::state &lua)
	{
		UserTypes::allGLM(lua);
		UserTypes::allNodes(lua);
	}
}