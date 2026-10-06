#pragma once
#include <glm/gtc/quaternion.hpp>
#include <serialisation/Serialisation.h>
#include <glm/gtx/quaternion.hpp>


namespace Tank
{


	/// @brief Stores the Entity model matrix in a friendly format.
	struct TransformComponent
	{
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 translation;

		TransformComponent() = default;
		TransformComponent(const TransformComponent &) = default;

		operator glm::mat4() { return getLocalModelMatrix(); }
		operator const glm::mat4() { return getLocalModelMatrix(); }

		glm::mat4 getLocalModelMatrix() const;
		glm::mat4 getWorldModelMatrix() const;
	};


	template <>
	json serialise<TransformComponent>(TransformComponent *);
	template <>
	TransformComponent deserialise(const json &);
}