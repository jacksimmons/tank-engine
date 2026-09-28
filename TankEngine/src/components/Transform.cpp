#include <Serialisation.h>
#include <static/GlmSerialise.h>
#include "Transform.h"


namespace Tank
{
	glm::mat4 TransformComponent::getLocalModelMatrix() const
	{
		glm::mat4 model = glm::identity<glm::mat4>();

		model = glm::translate(model, m_translation);
		model = glm::mat4_cast(m_rotation) * model;
		model = glm::scale(model, m_scale);

		return model;
	}


	json TransformComponent::serialise(TransformComponent *deserialised)
	{
		json serialised = {
			{ "rotation", quat::serialise(deserialised->m_rotation) },
			{ "scale", vec3::serialise(deserialised->m_scale) },
			{ "translation", vec3::serialise(deserialised->m_translation) },
		};

		return serialised;
	}


	void TransformComponent::deserialise(const json &serialised, TransformComponent *transform)
	{
		transform->setLocalRotation(quat::deserialise(serialised["rotation"]));
		transform->setLocalScale(vec3::deserialise(serialised["scale"]));
		transform->setLocalTranslation(vec3::deserialise(serialised["translation"]));
	}
}