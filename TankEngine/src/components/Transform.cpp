#include <glm/gtx/quaternion.hpp>
#include <static/GlmSerialise.h>
#include "Transform.h"


namespace Tank
{
	glm::mat4 TransformComponent::getLocalModelMatrix() const
	{
		glm::mat4 model = glm::identity<glm::mat4>();

		model = glm::translate(model, translation);
		model = glm::mat4_cast(rotation) * model;
		model = glm::scale(model, scale);

		return model;
	}


	// =======================
	//		Serialisation
	// =======================
	template <>
	json serialise<TransformComponent>(TransformComponent *deserialised)
	{
		json serialised = {
			{ "rotation", quat::serialise(deserialised->rotation) },
			{ "scale", vec3::serialise(deserialised->scale) },
			{ "translation", vec3::serialise(deserialised->translation) },
		};

		return serialised;
	}

	template <>
	TransformComponent deserialise(const json &serialised)
	{
		TransformComponent transform {};
		transform.rotation = quat::deserialise(serialised["rotation"]);
		transform.scale = vec3::deserialise(serialised["scale"]);
		transform.translation = vec3::deserialise(serialised["translation"]);
		return transform;
	}
}
