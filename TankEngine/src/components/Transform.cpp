#include <glm/gtx/quaternion.hpp>
#include <static/GlmSerialise.h>
#include "Transform.h"


namespace Tank
{
	glm::mat4 TransformComponent::getLocalModelMatrix() const
	{
		glm::mat4 model = glm::identity<glm::mat4>();

		model = glm::translate(model, Translation);
		model = glm::mat4_cast(Rotation) * model;
		model = glm::scale(model, Scale);

		return model;
	}


	// =======================
	//		Serialisation
	// =======================
	template <>
	json serialise(TransformComponent *deserialised)
	{
		json serialised = {
			{ "rotation", quat::serialise(deserialised->Rotation) },
			{ "scale", vec3::serialise(deserialised->Scale) },
			{ "translation", vec3::serialise(deserialised->Translation) },
		};

		return serialised;
	}

	template <>
	void deserialise(const json &serialised, TransformComponent *out)
	{
		out->Rotation = quat::deserialise(serialised["rotation"]);
		out->Scale = vec3::deserialise(serialised["scale"]);
		out->Translation = vec3::deserialise(serialised["translation"]);
	}
}