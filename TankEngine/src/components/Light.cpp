#include <Transformation.h>
#include <Shader.h>
#include <Log.h>
#include <static/GlmSerialise.h>
#include "Light.h"
#include "Model.h"
#include "Sprite.h"


namespace Tank
{
	// =======================
	//		Serialisation
	// =======================
	template <>
	json Tank::serialise<LightIntensity>(LightIntensity *in)
	{
		json serialised;
		serialised["ambient"] = vec3::serialise(in->ambient);
		serialised["diffuse"] = vec3::serialise(in->diffuse);
		serialised["specular"] = vec3::serialise(in->specular);
		return serialised;
	}
	
	template <>
	LightIntensity deserialise(const json &serialised)
	{
		return LightIntensity
		{
			vec3::deserialise(serialised["ambient"]),
			vec3::deserialise(serialised["diffuse"]),
			vec3::deserialise(serialised["specular"])
		};
	}


	template <>
	json Tank::serialise<DirectionalLightComponent>(DirectionalLightComponent *in)
	{
		json serialised;
		serialised["intensity"] = serialise(&in->intensity);
		serialised["direction"] = vec3::serialise(in->direction);
		return serialised;
	}

	template <>
	DirectionalLightComponent Tank::deserialise(const json &serialised)
	{
		DirectionalLightComponent dlc {};
		dlc.intensity = deserialise<LightIntensity>(serialised["intensity"]);
		dlc.direction = vec3::deserialise(serialised["direction"]);
		return dlc;
	}


	template <>
	json Tank::serialise<PointLightComponent>(PointLightComponent *in)
	{
		json serialised;
		serialised["intensity"] = serialise(&in->intensity);
		return serialised;
	}

	template <>
	PointLightComponent Tank::deserialise(const json &serialised)
	{
		PointLightComponent plc {};
		plc.intensity = deserialise<LightIntensity>(serialised["intensity"]);
		return plc;
	}
}
