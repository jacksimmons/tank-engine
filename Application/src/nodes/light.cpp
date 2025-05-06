#include "transform.hpp"
#include "shader.hpp"
#include "log.hpp"
#include "static/glm_serialise.hpp"
#include "nodes/light.hpp"
#include "nodes/model.hpp"
#include "nodes/scene.hpp"
#include "nodes/sprite.hpp"


namespace Tank
{
	json Light::serialise(Light *light)
	{
		json serialised = Node::serialise(light);
		serialised["ambient"] = vec3::serialise(light->getAmbient());
		serialised["diffuse"] = vec3::serialise(light->getDiffuse());
		serialised["specular"] = vec3::serialise(light->getSpecular());
		return serialised;
	}


	void Light::deserialise(const json &serialised, Light **targetPtr)
	{
		if (!(*targetPtr)) *targetPtr = new Light();

		Light *light = *targetPtr;
		light->setAmbient(vec3::deserialise(serialised["ambient"]));
		light->setDiffuse(vec3::deserialise(serialised["diffuse"]));
		light->setSpecular(vec3::deserialise(serialised["specular"]));

		Node *target = *targetPtr;
		Node::deserialise(serialised, &target);
	}


	Light::Light(const std::string &name, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) : Node(name),
		m_ambient(amb), m_diffuse(diff), m_specular(spec)
	{
		m_scene = Scene::getActiveScene();
	}


	Light::~Light()
	{
		// Scene may destroy this light first, e.g. with Open Scene
		m_scene->removeLight(this);
	}


	void Light::updateShader(Shader *shader)
	{
		std::string str = getLightStruct();
		if (getEnabled())
		{
			shader->setVec3(str + ".Ia", m_ambient);
			shader->setVec3(str + ".Id", m_diffuse);
			shader->setVec3(str + ".Is", m_specular);
		}
		else
		{
			shader->setVec3(str + ".Ia", glm::vec3(0.0f));
			shader->setVec3(str + ".Id", glm::vec3(0.0f));
			shader->setVec3(str + ".Is", glm::vec3(0.0f));
		}
	}


	std::string Light::getLightStruct()
	{
		auto lights = m_scene->getLights();
		auto it = std::find(lights.begin(), lights.end(), this);
		if (it != lights.end())
		{
			return m_lightArrayName + "[" + std::to_string(it - lights.begin()) + "]";
		}
		else
		{
			TE_CORE_ERROR("Failed to get light struct.");
		}
	}


	json DirLight::serialise(DirLight *light)
	{
		json serialised = Light::serialise(light);
		serialised["direction"] = vec3::serialise(light->getDirection());
		return serialised;
	}


	void DirLight::deserialise(const json &serialised, DirLight **targetPtr)
	{
		if (!(*targetPtr)) *targetPtr = new DirLight();

		DirLight *dirLight = *targetPtr;
		dirLight->setDirection(vec3::deserialise(serialised["direction"]));

		Light *target = *targetPtr;
		Light::deserialise(serialised, &target);
	}


	DirLight::DirLight(const std::string &name, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
		: Light(name, amb, diff, spec), m_direction(dir)
	{
		m_type = "DirLight";
		m_lightArrayName = "dirLights";

		if (m_scene->getNumLights(LightType::Directional) >= 64)
		{
			TE_CORE_WARN("Reached limit of DirLight lights; this light will not apply to shaders.");
			return;
		}

		m_scene->addLight(this);
	}


	DirLight::~DirLight()
	{
	}


	void DirLight::updateShader(Shader *shader)
	{
		std::string str = getLightStruct();
		shader->setVec3(str + ".dir", m_direction);

		Light::updateShader(shader);
	}


	PointLight::PointLight(const std::string &name, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
		: Light(name, amb, diff, spec)
	{
		m_type = "PointLight";
		m_lightArrayName = "pointLights";

		if (m_scene->getNumLights(LightType::Point) >= 64)
		{
			TE_CORE_WARN("Reached limit of PointLight lights; this light will not apply to shaders.");
			return;
		}

		m_scene->addLight(this);
	}


	PointLight::~PointLight()
	{
	}


	void PointLight::updateShader(Shader *shader)
	{
		std::string str = getLightStruct();
		shader->setVec3(str + ".pos", mat4::getTranslation(getTransform()->getWorldModelMatrix()));
		shader->setFloat(str + ".constant", 1.0f);
		shader->setFloat(str + ".linear", 0.0f);
		shader->setFloat(str + ".quadratic", 0.0f);

		Light::updateShader(shader);
	}
}