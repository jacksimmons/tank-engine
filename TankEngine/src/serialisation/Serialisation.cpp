#include <static/GlmSerialise.h>
#include "Serialisation.h"


namespace Tank
{
	template <>
	json serialise(TransformComponent *deserialised)
	{
		json serialised = {
			{ "rotation", quat::serialise(deserialised->m_rotation) },
			{ "scale", vec3::serialise(deserialised->m_scale) },
			{ "translation", vec3::serialise(deserialised->m_translation) },
		};

		return serialised;
	}

	template <>
	void deserialise(const json &serialised, TransformComponent *transform)
	{
		transform->setLocalRotation(quat::deserialise(serialised["rotation"]));
		transform->setLocalScale(vec3::deserialise(serialised["scale"]));
		transform->setLocalTranslation(vec3::deserialise(serialised["translation"]));
	}
}