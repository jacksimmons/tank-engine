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
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};


	struct DirectionalLightComponent
	{
	public:
		LightIntensity intensity =
		{
			{ 0.02f, 0.02f, 0.02f },
			{ 0.1f, 0.1f, 0.1f },
			{ 0.2f, 0.2f, 0.2f }
		};
		glm::vec3 direction = { 0.0f, -1.0f, 0.0f };

		DirectionalLightComponent() = default;
		~DirectionalLightComponent() = default;
	};


	struct PointLightComponent
	{
	public:
		LightIntensity intensity =
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
	LightIntensity deserialise(const json &);

	template <>
	json serialise<DirectionalLightComponent>(DirectionalLightComponent *);
	template <>
	DirectionalLightComponent deserialise(const json &);

	template <>
	json serialise<PointLightComponent>(PointLightComponent *);
	template <>
	PointLightComponent deserialise(const json &);
}
