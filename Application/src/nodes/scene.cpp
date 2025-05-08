#include "log.hpp"
#include "nodes/scene.hpp"
#include "nodes/camera.hpp"
#include "nodes/light.hpp"


namespace Tank
{
	json Scene::serialise()
	{
		json serialised = Node::serialise();
		serialised["activeCam"] = treeFromChild(getActiveCamera());
		serialised["isActiveScene"] = Scene::getActiveScene() == this;
		return serialised;
	}


	void Scene::deserialise(const json &serialised, Scene **targetPtr)
	{
		if (!(*targetPtr)) *targetPtr = new Scene();

		Scene *scene = *targetPtr;
		if (serialised["isActiveScene"]) Scene::setActiveScene(scene);
		scene->setActiveCamera(dynamic_cast<Camera*>(scene->childFromTree(serialised["activeCam"])));
		
		Node *target = *targetPtr;
		Node::deserialise(serialised, &target);
	}


	Scene *Scene::s_activeScene = nullptr;


	Scene::Scene(const std::string &name) : Node(name)
	{
		m_type = "Scene";
		m_activeCamera = nullptr;
	}


	void Scene::update()
	{
		// Ignore updates if no camera is active.
		if (!m_activeCamera || !m_activeCamera->getEnabled()) return;

		// Update camera, then the scene
		m_activeCamera->update();
		Node::update();
	}


	unsigned Scene::addLight(Light *light)
	{
		auto it = std::find(m_lights.begin(), m_lights.end(), light);
		if (it != m_lights.end())
		{
			TE_CORE_WARN("Light has already been added to this scene.");
		}

		m_lights.push_back(light);
		return m_lights.size() - 1;
	}


	void Scene::removeLight(Light *light)
	{
		auto it = std::find(m_lights.begin(), m_lights.end(), light);
		if (it != m_lights.end())
		{
			m_lights.erase(it);
		}
		else
		{
			TE_CORE_WARN("Light was not found in this scene.");
		}
	}


	unsigned Scene::getNumLights(LightType type) const
	{
		unsigned cnt = 0;

		switch (type)
		{
		case LightType::Any:
			cnt = m_lights.size();
			break;
		default:
			for (Light *light : m_lights)
			{
				if (light->getType() == type) cnt++;
			}
			break;
		}

		return cnt;
	}


	void Scene::onNodeDeleted(Node *deleted) noexcept
	{
		if (m_activeCamera == deleted)
		{
			m_activeCamera = nullptr;
			return;
		}
	}
}