#pragma once
#include <serialisation/Serialisation.h>


namespace Tank
{
	class Shader;
	struct ShaderSource;
	struct ShaderSources;
	class GlobalLight;
	class Scene;


	enum class LightType
	{
		Any,
		Point,
		Directional
	};


	// Names of the arrays in GLSL containing all light structs.
	namespace LIGHT
	{
		const std::string DIRECTIONAL_ARRAY = "dirLights";
		const unsigned DIRECTIONAL_ARRAY_SIZE = 64;

		const std::string POINT_ARRAY = "pointLights";
		const unsigned POINT_ARRAY_SIZE = 64;
	}


	struct LightIntensity
	{
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
	};


	struct DirectionalLightComponent
	{
	public:
		LightIntensity Intensity =
		{
			{ 0.02f, 0.02f, 0.02f },
			{ 0.1f, 0.1f, 0.1f },
			{ 0.2f, 0.2f, 0.2f }
		};
		glm::vec3 Direction = { 0.0f, -1.0f, 0.0f };

		DirectionalLightComponent() = default;
		~DirectionalLightComponent() = default;
	};


	struct PointLightComponent
	{
	public:
		LightIntensity Intensity =
		{
			{ 0.1f, 0.1f, 0.1f },
			{ 0.1f, 0.1f, 0.1f },
			{ 0.1f, 0.1f, 0.1f }
		};

		PointLightComponent() = default;
		~PointLightComponent() = default;
	};


	template <>
	json serialise<LightIntensity>(LightIntensity *);
	template <>
	void deserialise<LightIntensity>(const json &, LightIntensity *);

	template <>
	json serialise<DirectionalLightComponent>(DirectionalLightComponent *);
	template <>
	void deserialise<DirectionalLightComponent>(const json &, DirectionalLightComponent *);

	template <>
	json serialise<PointLightComponent>(PointLightComponent *);
	template <>
	void deserialise<PointLightComponent>(const json &, PointLightComponent *);
}
