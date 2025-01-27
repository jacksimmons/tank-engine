#include <stack>
#include <iostream>
#include "log.hpp"
#include "scene.hpp"
#include "nodes/node.hpp"
#include "nodes/camera.hpp"
#include "nodes/light.hpp"


namespace Tank
{
	Scene *Scene::s_activeScene = nullptr;

	Scene::Scene(std::unique_ptr<Node> root, Camera *cam)
	{
		m_root = std::move(root);
		m_activeCamera = cam;
	}

	Scene::~Scene()
	{
		m_root = nullptr;
	}

	void Scene::update()
	{
		// Ignore rendering updates if no camera is active.
		if (m_activeCamera == nullptr) return;

		m_root->update();
		m_activeCamera->update();
	}

	void Scene::forEachNode(std::function<void(Node *)> forEach) const
	{
		Node *node = getRoot();

		std::stack<Node *> nodeStack;
		nodeStack.push(node);

		while (!nodeStack.empty())
		{
			// Pop the top node from the stack, and perform `forEach`.
			node = nodeStack.top();
			nodeStack.pop();
			forEach(node);
			TE_CORE_INFO(node->getName());

			// Add all its children to the stack.
			for (int i = 0; i < node->getChildCount(); i++)
			{
				nodeStack.push(node->getChild(i));
			}
		}
	}

	void Scene::updateShaders() const
	{
		forEachNode([this](Node *node)
		{
			for (Light *light : m_activeLights)
			{
				if (Model *model = dynamic_cast<Model *>(node))
				{
					Shader *shader = model->getShader();
					light->updateShader(shader);
				}
			}
		});
	}

	void Scene::addLight(Light *light)
	{
		if (std::find(m_activeLights.begin(), m_activeLights.end(), light) != m_activeLights.end())
		{
			TE_CORE_WARN("Light has already been added to this scene.");
		}
		
		m_activeLights.push_back(light);
		updateShaders();
	}

	void Scene::removeLight(Light *light)
	{
		auto it = std::find(m_activeLights.begin(), m_activeLights.end(), light);
		if (it != m_activeLights.end())
		{
			m_activeLights.erase(it);
			updateShaders();
		}
		else
		{
			TE_CORE_WARN("Light was not found in this scene.");
		}
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