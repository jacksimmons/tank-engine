#include "Log.h"
#include "nodes/Scene.h"
#include "nodes/Camera.h"
#include "nodes/Light.h"


namespace Tank
{
	_Scene *_Scene::s_activeScene = nullptr;


	_Scene::_Scene(const std::string &name) : Node(name)
	{
		m_type = "Scene";
		m_activeCamera = nullptr;
	}


	void _Scene::update()
	{
		// Ignore updates if no camera is active.
		if (!m_activeCamera || !m_activeCamera->Enabled()) return;

		// Update camera, then the scene
		m_activeCamera->update();
		Node::update();
	}


	unsigned _Scene::addLight(Light *light)
	{
		auto it = std::find(m_lights.begin(), m_lights.end(), light);
		if (it != m_lights.end())
		{
			TE_CORE_WARN("Light has already been added to this scene.");
		}

		m_lights.push_back(light);
		return m_lights.size() - 1;
	}


	void _Scene::removeLight(Light *light)
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


	unsigned _Scene::getNumLights(LightType type) const
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


	void _Scene::onNodeDeleted(Node *deleted) noexcept
	{
		if (m_activeCamera == deleted)
		{
			m_activeCamera = nullptr;
			return;
		}
	}
}


json Tank::_Scene::serialise()
{
	json serialised = Node::serialise();
	serialised["activeCam"] = treeFromChild(getActiveCamera());
	serialised["isActiveScene"] = _Scene::getActiveScene() == this;
	return serialised;
}

void Tank::_Scene::deserialise(const json &serialised)
{
	if (serialised["isActiveScene"]) _Scene::setActiveScene(this);
	setActiveCamera(dynamic_cast<Camera*>(childFromTree(serialised["activeCam"])));

	Node::deserialise(serialised);
}