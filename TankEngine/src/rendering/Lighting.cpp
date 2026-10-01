#include <components/Light.h>
#include <Shader.h>
#include <Log.h>
#include "Lighting.h"


namespace Tank
{
	void Lighting::applyLightIntensityToShader(const LightIntensity &I, const std::string &lightElement, const Shader &shader)
	{
		shader.setVec3(lightElement + ".Ia", I.Ambient);
		shader.setVec3(lightElement + ".Id", I.Diffuse);
		shader.setVec3(lightElement + ".Is", I.Specular);
	}


	void Lighting::applyDirectionalLightToShader(const DirectionalLightComponent &dirLight, const Shader &shader, unsigned index)
	{
		std::string lightArrayName = LIGHT::DIRECTIONAL_ARRAY;
		std::string lightElement = std::format("{}[{}]", lightArrayName, std::to_string(index));

		Lighting::applyLightIntensityToShader(dirLight.Intensity, lightElement, shader);
		shader.setVec3(lightElement + ".dir", dirLight.Direction);
	}


	void Lighting::applyPointLightToShader(const PointLightComponent &ptLight, const Shader &shader, unsigned index)
	{
		std::string lightArrayName = LIGHT::POINT_ARRAY;
		std::string lightElement = std::format("{}[{}]", lightArrayName, std::to_string(index));

		Lighting::applyLightIntensityToShader(ptLight.Intensity, lightElement, shader);
		shader.setVec3(lightElement + ".pos", glm::vec3(0.0f));
		shader.setFloat(lightElement + ".constant", 1.0f);
		shader.setFloat(lightElement + ".linear", 0.0f);
		shader.setFloat(lightElement + ".quadratic", 0.0f);
	}
}