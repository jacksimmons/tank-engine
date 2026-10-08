#include <Transformation.h>
#include <Shader.h>
#include <Log.h>
#include <serialisation/GlmSerialisation.h>
#include "Light.h"
#include "Model.h"
#include "Sprite.h"


namespace Tank
{
	// =======================
	//		Serialisation
	// =======================
	template <>
	json Serialisation::serialise<LightIntensity>(LightIntensity *in)
	{
		json serialised;
		serialised["ambient"] = vec3::serialise(in->ambient);
		serialised["diffuse"] = vec3::serialise(in->diffuse);
		serialised["specular"] = vec3::serialise(in->specular);
		return serialised;
	}
	
	template <>
	LightIntensity Serialisation::deserialise(const json &serialised)
	{
		return LightIntensity
		{
			vec3::deserialise(serialised["ambient"]),
			vec3::deserialise(serialised["diffuse"]),
			vec3::deserialise(serialised["specular"])
		};
	}
}
