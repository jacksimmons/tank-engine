#pragma once

#include <glm/gtx/quaternion.hpp>
#include <core.hpp>
#include "transformation.hpp"


namespace Tank
{
	class Node;
	class TANK_API Transform
	{
		friend class Node;
	public:
		static json serialise(Transform *deserialised);
		static void deserialise(const json &serialised, Transform *transform);

	private:
		// Entire transformation
		glm::mat4 m_modelMatrix;
		Node *m_owner;

		// Individual transformation components
		glm::quat m_rotation;
		glm::vec3 m_scale;
		glm::vec3 m_translation;

	public:
		Transform(Node *owner = nullptr);
		virtual ~Transform() {};

		/// <summary>
		/// Recursively pre-multiplies the model matrix by parent's getWorldMatrix result.
		/// </summary>
		glm::mat4 getWorldModelMatrix() const;
		glm::mat4 getLocalModelMatrix() const;

		const glm::quat& getLocalRotation() const { return m_rotation; }
		const glm::vec3& getLocalScale() const { return m_scale; }
		const glm::vec3& getLocalTranslation() const { return m_translation; }

		void setLocalRotation(const glm::quat &rot) { m_rotation = rot; }
		void setLocalScale(const glm::vec3 &scale) { m_scale = scale; }
		void setLocalTranslation(const glm::vec3 &trans) { m_translation = trans; }
		void setOwner(Node *owner) { m_owner = owner; }
	};
}