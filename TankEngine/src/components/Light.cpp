#include <Transformation.h>
#include <Shader.h>
#include <Log.h>
#include <static/GlmSerialise.h>
#include "Light.h"
#include "Model.h"
#include "Scene.h"
#include "Sprite.h"


namespace Tank
{
	LightComponent::LightComponent(const std::string &name, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) :
		m_ambient(amb), m_diffuse(diff), m_specular(spec)
	{
		m_scene = Scene::getActiveScene();

		// Add the light to scene
		if (m_scene && m_scene->getNumLights(getType()) >= 64)
		{
			TE_CORE_WARN("Reached the light limit; this light will not apply to shaders.");
			return;
		}
		if (m_scene) m_scene->addLight(this);
	}


	LightComponent::~LightComponent()
	{
		// Scene may destroy this light first, e.g. with Open Scene
		if (m_scene) m_scene->removeLight(this);
	}


	void LightComponent::updateShader(const Shader &shader)
	{
		std::string str = getLightStruct();
		shader.setVec3(str + ".Ia", m_ambient);
		shader.setVec3(str + ".Id", m_diffuse);
		shader.setVec3(str + ".Is", m_specular);
	}


	std::string LightComponent::getLightStruct()
	{
		auto lights = m_scene->getLights();
		auto it = std::find(lights.begin(), lights.end(), this);
		if (it != lights.end())
		{
			return m_lightArrayName + "[" + std::to_string(m_scene->getNumLights(getType())-1) + "]";
		}
		else
		{
			TE_CORE_ERROR("Failed to get light struct.");
			return "";
		}
	}


	LightType LightComponent::getType()
	{
		if (dynamic_cast<PointLightComponent*>(this))
		{
			return LightType::Point;
		}
		else if (dynamic_cast<DirLightComponent*>(this))
		{
			return LightType::Directional;
		}

		return LightType::Any;
	}


	DirLightComponent::DirLightComponent(const std::string &name, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
		: LightComponent(name, amb, diff, spec), m_direction(dir)
	{
		m_lightArrayName = "dirLights";
	}


	DirLightComponent::~DirLightComponent()
	{
	}


	void DirLightComponent::updateShader(const Shader &shader)
	{
		std::string str = getLightStruct();
		shader.setVec3(str + ".dir", m_direction);

		LightComponent::updateShader(shader);
	}


	PointLightComponent::PointLightComponent(const std::string &name, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
		: LightComponent(name, amb, diff, spec)
	{
		m_lightArrayName = "pointLights";
	}


	PointLightComponent::~PointLightComponent()
	{
	}


	void PointLightComponent::updateShader(const Shader &shader)
	{
		std::string str = getLightStruct();
		shader.setVec3(str + ".pos", glm::vec3(0.0f));
		shader.setFloat(str + ".constant", 1.0f);
		shader.setFloat(str + ".linear", 0.0f);
		shader.setFloat(str + ".quadratic", 0.0f);

		LightComponent::updateShader(shader);
	}


	// =======================
	//		Serialisation
	// =======================
	template <>
	json Tank::serialise<LightComponent>(LightComponent *in)
	{
		json serialised;
		serialised["ambient"] = vec3::serialise(in->getAmbient());
		serialised["diffuse"] = vec3::serialise(in->getDiffuse());
		serialised["specular"] = vec3::serialise(in->getSpecular());
		return serialised;
	}

	template <>
	void Tank::deserialise<LightComponent>(const json &serialised, LightComponent *out)
	{
		out->setAmbient(vec3::deserialise(serialised["ambient"]));
		out->setDiffuse(vec3::deserialise(serialised["diffuse"]));
		out->setSpecular(vec3::deserialise(serialised["specular"]));
	}

	template <>
	json Tank::serialise<DirLightComponent>(DirLightComponent *in)
	{
		json serialised = serialise(static_cast<LightComponent *>(in));
		serialised["direction"] = vec3::serialise(in->getDirection());
		return serialised;
	}

	template <>
	void Tank::deserialise<DirLightComponent>(const json &serialised, DirLightComponent *out)
	{
		out->setDirection(vec3::deserialise(serialised["direction"]));
		deserialise(serialised, static_cast<LightComponent *>(out));
	}
}
