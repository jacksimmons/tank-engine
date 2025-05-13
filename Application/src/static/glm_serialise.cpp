#include <glm/ext/matrix_transform.hpp>
#include "glm_serialise.hpp"
#include "transformation.hpp"


namespace Tank
{
	// ============== glm::vec3 ===============
	json vec3::serialise(const glm::vec3 &vec3)
	{
		json serialised = {
			{ "x", vec3.x },
			{ "y", vec3.y },
			{ "z", vec3.z }
		};

		return serialised;
	}


	glm::vec3 vec3::deserialise(const json &serialised)
	{
		return { serialised["x"], serialised["y"], serialised["z"] };
	}


	// ============== glm::vec4 ===============
	json vec4::serialise(const glm::vec4 &vec4)
	{
		json serialised = {
			{ "x", vec4.x },
			{ "y", vec4.y },
			{ "z", vec4.z },
			{ "w", vec4.w },
		};

		return serialised;
	}


	glm::vec4 vec4::deserialise(const json &serialised)
	{
		return { serialised["x"], serialised["y"], serialised["z"], serialised["w"] };
	}


	// ============== glm::quat ===============
	json quat::serialise(const glm::quat &quat)
	{
		json serialised = {
			{ "w", quat.w },
			{ "x", quat.x },
			{ "y", quat.y },
			{ "z", quat.z }
		};

		return serialised;
	}


	glm::quat quat::deserialise(const json &serialised)
	{
		return glm::quat{ serialised["w"], serialised["x"], serialised["y"], serialised["z"] };
	}


	// ============== glm::quat ===============
	json mat4::serialise(const glm::mat4 &mat4)
	{
		json serialised = {
			vec4::serialise(mat4[0]),
			vec4::serialise(mat4[1]),
			vec4::serialise(mat4[2]),
			vec4::serialise(mat4[3]),
		};
		return serialised;
	}


	glm::mat4 mat4::deserialise(const json &serialised)
	{
		glm::mat4 mat4 = {
			vec4::deserialise(serialised[0]),
			vec4::deserialise(serialised[1]),
			vec4::deserialise(serialised[2]),
			vec4::deserialise(serialised[3]),
		};
		return mat4;
	}
}