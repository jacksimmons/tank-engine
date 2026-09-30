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
		serialised["ambient"] = vec3::serialise(in->Ambient);
		serialised["diffuse"] = vec3::serialise(in->Diffuse);
		serialised["specular"] = vec3::serialise(in->Specular);
		return serialised;
	}
	
	template <>
	void deserialise<LightIntensity>(const json &serialised, LightIntensity *out)
	{
		out->Ambient = vec3::deserialise(serialised["ambient"]);
		out->Diffuse = vec3::deserialise(serialised["diffuse"]);
		out->Specular = vec3::deserialise(serialised["specular"]);
	}


	template <>
	json Tank::serialise<DirectionalLightComponent>(DirectionalLightComponent *in)
	{
		json serialised;
		serialised["intensity"] = serialise(&in->Intensity);
		serialised["direction"] = vec3::serialise(in->Direction);
		return serialised;
	}

	template <>
	void Tank::deserialise<DirectionalLightComponent>(const json &serialised, DirectionalLightComponent *out)
	{
		deserialise(serialised["intensity"], &out->Intensity);
		out->Direction = vec3::deserialise(serialised["direction"]);
	}


	template <>
	json Tank::serialise<PointLightComponent>(PointLightComponent *in)
	{
		json serialised;
		serialised["intensity"] = serialise(&in->Intensity);
		return serialised;
	}

	template <>
	void Tank::deserialise<PointLightComponent>(const json &serialised, PointLightComponent *out)
	{
		deserialise(serialised["intensity"], &out->Intensity);
	}
}
