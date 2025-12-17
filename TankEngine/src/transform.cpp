#include "transformation.hpp"
#include "nodes/node.hpp"
#include "static/glm_serialise.hpp"


namespace Tank
{
	json Transform::serialise(Transform *deserialised)
	{
		json serialised = {
			{ "rotation", quat::serialise(deserialised->m_rotation) },
			{ "scale", vec3::serialise(deserialised->m_scale) },
			{ "translation", vec3::serialise(deserialised->m_translation) },
		};

		return serialised;
	}


	void Transform::deserialise(const json &serialised, Transform *transform)
	{
		transform->setLocalRotation(quat::deserialise(serialised["rotation"]));
		transform->setLocalScale(vec3::deserialise(serialised["scale"]));
		transform->setLocalTranslation(vec3::deserialise(serialised["translation"]));
	}


	Transform::Transform(Node *owner)
	{
		m_modelMatrix = glm::identity<glm::mat4>();
		m_scale = { 1,1,1 };
		m_rotation = glm::identity<glm::quat>();
		m_translation = { 0,0,0 };
		m_owner = owner;
	}


	glm::mat4 Transform::getLocalModelMatrix() const
	{
		glm::mat4 model = glm::identity<glm::mat4>();

		model = glm::translate(model, m_translation);
		model = glm::mat4_cast(m_rotation) * model;
		model = glm::scale(model, m_scale);

		return model;
	}


	glm::mat4 Transform::getWorldModelMatrix() const
	{
		Node *parent = m_owner->getParent();
		if (parent != nullptr)
		{
			return (parent->getTransform()->getWorldModelMatrix())
			 * getLocalModelMatrix();
		}

		return getLocalModelMatrix();
	}
}