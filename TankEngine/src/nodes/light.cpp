#include "shader.hpp"
#include "log.hpp"
#include "static/glm_serialise.hpp"
#include "nodes/light.hpp"
#include "nodes/model.hpp"
#include "nodes/scene.hpp"
#include "nodes/sprite.hpp"


namespace Tank
{
	Light::Light(const std::string &name, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) :
		Node(name),
		m_ambient(amb), m_diffuse(diff), m_specular(spec)
	{
		m_scene = Scene::getActiveScene();

		// Add the light to scene
		if (m_scene->getNumLights(getType()) >= 64)
		{
			TE_CORE_WARN(std::format("Reached limit of {} lights; this light will not apply to shaders.", m_type));
			return;
		}
		m_scene->addLight(this);

		auto gizmo = std::make_unique<Sprite>("Gizmo", fs::current_path() / "textures/gizmo/dir_light.png");
		addChild(std::move(gizmo));
	}


	Light::~Light()
	{
		// Scene may destroy this light first, e.g. with Open Scene
		m_scene->removeLight(this);
	}


	void Light::updateShader(const Shader &shader)
	{
		std::string str = getLightStruct();
		if (Enabled())
		{
			shader.setVec3(str + ".Ia", m_ambient);
			shader.setVec3(str + ".Id", m_diffuse);
			shader.setVec3(str + ".Is", m_specular);
		}
		else
		{
			shader.setVec3(str + ".Ia", glm::vec3(0.0f));
			shader.setVec3(str + ".Id", glm::vec3(0.0f));
			shader.setVec3(str + ".Is", glm::vec3(0.0f));
		}
	}


	std::string Light::getLightStruct()
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


	LightType Light::getType()
	{
		if (dynamic_cast<PointLight*>(this))
		{
			return LightType::Point;
		}
		else if (dynamic_cast<DirLight*>(this))
		{
			return LightType::Directional;
		}

		return LightType::Any;
	}


	DirLight::DirLight(const std::string &name, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
		: Light(name, amb, diff, spec), m_direction(dir)
	{
		m_type = "DirLight";
		m_lightArrayName = "dirLights";
	}


	DirLight::~DirLight()
	{
	}


	void DirLight::updateShader(const Shader &shader)
	{
		std::string str = getLightStruct();
		shader.setVec3(str + ".dir", m_direction);

		Light::updateShader(shader);
	}


	PointLight::PointLight(const std::string &name, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
		: Light(name, amb, diff, spec)
	{
		m_type = "PointLight";
		m_lightArrayName = "pointLights";
	}


	PointLight::~PointLight()
	{
	}


	void PointLight::updateShader(const Shader &shader)
	{
		std::string str = getLightStruct();
		shader.setVec3(str + ".pos", mat4::getTranslation(getTransform()->getWorldModelMatrix()));
		shader.setFloat(str + ".constant", 1.0f);
		shader.setFloat(str + ".linear", 0.0f);
		shader.setFloat(str + ".quadratic", 0.0f);

		Light::updateShader(shader);
	}
}


json Tank::Light::serialise()
{
	json serialised = Node::serialise();
	serialised["ambient"] = vec3::serialise(getAmbient());
	serialised["diffuse"] = vec3::serialise(getDiffuse());
	serialised["specular"] = vec3::serialise(getSpecular());
	return serialised;
}

void Tank::Light::deserialise(const json &serialised)
{
	setAmbient(vec3::deserialise(serialised["ambient"]));
	setDiffuse(vec3::deserialise(serialised["diffuse"]));
	setSpecular(vec3::deserialise(serialised["specular"]));

	Node::deserialise(serialised);
}


json Tank::DirLight::serialise()
{
	json serialised = Light::serialise();
	serialised["direction"] = vec3::serialise(getDirection());
	return serialised;
}

void Tank::DirLight::deserialise(const json &serialised)
{
	setDirection(vec3::deserialise(serialised["direction"]));
	Light::deserialise(serialised);
}