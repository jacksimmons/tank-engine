#include "user_types.hpp"
#include "sol/sol.hpp"
#include <GLFW/glfw3.h>
// The types to-be-defined as usertypes
#include <glm/glm.hpp>
#include <transform.hpp>
#include <key_input.hpp>
#include <nodes/node.hpp>
#include <nodes/camera.hpp>


#define KC_PAIR(x) #x, GLFW_KEY_##x


namespace Tank
{
	void UserTypes::allGLM(sol::state &lua)
	{
		sol::usertype<glm::vec3> vec3 = lua.new_usertype<glm::vec3>(
			"Vec3",
			sol::constructors<glm::vec3(), glm::vec3(float, float, float)>()
		);
		vec3["x"] = &glm::vec3::x;
		vec3["y"] = &glm::vec3::y;
		vec3["z"] = &glm::vec3::z;
	}


	void UserTypes::allNodes(sol::state &lua)
	{
		// Define types Node is dependent on

		// Transform
		sol::usertype<Transform> utTransform = lua.new_usertype<Transform>("Transform");
		utTransform["translation"] = sol::property(&Transform::getLocalTranslation, &Transform::setLocalTranslation);
		utTransform["rotation"] = sol::property(&Transform::getLocalRotation, &Transform::setLocalRotation);

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

		sol::usertype<KeyInput> utKeyInput = lua.new_usertype<KeyInput>("KeyInput");
		utKeyInput["get_key_state"] = &KeyInput::getKeyState;

		// Now we can define Node and its subclasses
		sol::usertype<Node> utNode = lua.new_usertype<Node>("Node");
		utNode["name"] = sol::property(&Node::getName, &Node::setName);
		utNode["transform"] = sol::property(&Node::getTransform);
		utNode["key_input"] = sol::property(&Node::getKeyInput);
		utNode["destroy"] = &Node::destroy;

		sol::usertype<Camera> utCamera = lua.new_usertype<Camera>(
			"Camera",
			sol::base_classes, sol::bases<Node>()
		);
		utCamera["set_pos"] = &Camera::setPosition;
	}


	void UserTypes::all(sol::state &lua)
	{
		UserTypes::allGLM(lua);
		UserTypes::allNodes(lua);
	}
}