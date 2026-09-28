#pragma once
#include <glm/gtx/quaternion.hpp>


namespace Tank
{
	/// @brief Stores the Entity model matrix in a friendly format.
	struct TANK_API TransformComponent
	{
		glm::quat m_rotation;
		glm::vec3 m_scale;
		glm::vec3 m_translation;

		TransformComponent() = default;
		TransformComponent(const TransformComponent &) = default;

		operator glm::mat4() { return getLocalModelMatrix(); }
		operator const glm::mat4() { return getLocalModelMatrix(); }

		glm::mat4 getLocalModelMatrix() const;
		glm::mat4 getWorldModelMatrix() const;

		const glm::quat &getLocalRotation() const { return m_rotation; }
		const glm::vec3 &getLocalScale() const { return m_scale; }
		const glm::vec3 &getLocalTranslation() const { return m_translation; }

		void setLocalRotation(const glm::quat &rot) { m_rotation = rot; }
		void setLocalScale(const glm::vec3 &scale) { m_scale = scale; }
		void setLocalTranslation(const glm::vec3 &trans) { m_translation = trans; }

		static json serialise(TransformComponent *deserialised);
		static void deserialise(const json &serialised, TransformComponent *transform);
	};
}