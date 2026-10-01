#pragma once


namespace Tank
{
	struct LightIntensity;
	struct DirectionalLightComponent;
	class Shader;


	class Lighting
	{
	public:
		static void applyLightIntensityToShader(const LightIntensity &I, const std::string &elementName, const Shader &shader);

		static void applyDirectionalLightToShader(const DirectionalLightComponent &dirLight, const Shader &shader, unsigned index);
		static void applyPointLightToShader(const PointLightComponent &ptLight, const Shader &shader, unsigned index);
	};
}