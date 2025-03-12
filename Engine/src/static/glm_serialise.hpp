#pragma once
#include "log.hpp"


namespace Tank
{
	namespace vec3
	{
		TANK_API json serialise(const glm::vec3 &vec3);
		TANK_API glm::vec3 deserialise(const json &json);
	}
	namespace vec4
	{
		TANK_API json serialise(const glm::vec4 &vec4);
		TANK_API glm::vec4 deserialise(const json &json);
	}
	namespace mat4
	{
		TANK_API json serialise(const glm::mat4 &mat4);
		TANK_API glm::mat4 deserialise(const json &json);
	}
	namespace quat
	{
		TANK_API json serialise(const glm::quat &quat);
		TANK_API glm::quat deserialise(const json &json);
	}
}