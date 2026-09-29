#pragma once
#include <serialisation/Serialisation.h>


namespace Tank
{
	/// @brief Stores the Entity model matrix in a friendly format.
	struct TransformComponent
	{
		glm::quat Rotation;
		glm::vec3 Scale;
		glm::vec3 Translation;

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
	void deserialise<TransformComponent>(const json &, TransformComponent *);
}