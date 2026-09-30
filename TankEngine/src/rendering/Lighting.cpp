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


	void Lighting::applyLightsToShader(const DirectionalLightComponent &light, unsigned index, const Shader &shader)
	{
		if (index >= LIGHT::DIRECTIONAL_ARRAY_SIZE)
		{
			TE_CORE_ERROR("Directional light limit exceeded!");
			return;
		}

		std::string lightArrayName = LIGHT::DIRECTIONAL_ARRAY;
		std::string lightElement = std::format("{}[{}]", lightArrayName, std::to_string(index));

		Lighting::applyLightIntensityToShader(light.Intensity, lightElement, shader);
		shader.setVec3(lightElement + ".dir", light.Direction);
	}


	void Lighting::applyLightsToShader(const PointLightComponent &light, unsigned index, const Shader &shader)
	{
		if (index >= LIGHT::POINT_ARRAY_SIZE)
		{
			TE_CORE_ERROR("Point light limit exceeded!");
			return;
		}

		std::string lightArrayName = LIGHT::POINT_ARRAY;
		std::string lightElement = std::format("{}[{}]", lightArrayName, std::to_string(index));

		Lighting::applyLightIntensityToShader(light.Intensity, lightElement, shader);
		shader.setVec3(lightElement + ".pos", glm::vec3(0.0f));
		shader.setFloat(lightElement + ".constant", 1.0f);
		shader.setFloat(lightElement + ".linear", 0.0f);
		shader.setFloat(lightElement + ".quadratic", 0.0f);
	}
}