#pragma once
#include <entt/entt.hpp>


namespace Tank
{
	class Shader;
	struct DirectionalLightComponent;


	class Lighting
	{
	private:
		static void applyLightIntensityToShader(const LightIntensity &I, const std::string &elementName, const Shader &shader);
	public:
		static void applyLightsToShader(std::vector<const DirectionalLightComponent &> lights, unsigned index, const Shader &shader);
		static void applyLightsToShader(std::vector<const PointLightComponent &> lights, unsigned index, const Shader &shader);
	};
}