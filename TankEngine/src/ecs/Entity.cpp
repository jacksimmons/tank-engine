#include "Entity.h"
#include "ECS.h"
#include <components/Transform.h>
#include <components/Tree.h>
#include <events/EventManager.h>
#include <KeyInput.h>


namespace Tank
{
	Entity::Entity(const entt::entity handle, ECS *ecs, const std::string &name)
		: m_handle(handle), m_ecs(ecs), m_name(name)
	{
	}


	void Entity::setName(const std::string &name) noexcept
	{
		m_name = name;

		// Get a vector of all sibling names
		std::vector<std::string> siblingNames;
		auto &treeComponent = tree();
		for (const auto *sibling : treeComponent.getSiblings())
		{
			if (sibling == this) continue;
			siblingNames.push_back(sibling->m_name);
		}

		std::vector<Entity*> siblings = treeComponent.getSiblings();
		std::function<Entity*()> findSiblingWithSameName = [this, &siblings]()
		{
			auto it = std::find_if(siblings.begin(), siblings.end(),
				[this](const Entity *sibling)
			{
				return this != sibling && sibling->m_name == m_name;
			});

			if (it != siblings.end()) return *it;
			return (Entity*)nullptr;
		};

		// If a sibling already uses this name, add a (1) [or (2) or (3) ... if necessary]
		if (findSiblingWithSameName() != nullptr)
		{
			int dupeIndex;
			std::string baseName = m_name;
			std::string dupeName;

			// Try each of NAME (1), NAME (2), ... NAME (NUM_SIBLINGS - 1)
			for (dupeIndex = 0; dupeIndex < tree().getSiblingCount(); dupeIndex++)
			{
				dupeName = std::format("{} ({})", baseName, dupeIndex);
				m_name = dupeName;
				if (findSiblingWithSameName() == nullptr) break;
			}

			// If even NAME (NUM_SIBLINGS - 1) didn't work, we have a problem
			if (dupeIndex == tree().getSiblingCount())
			{
				TE_CORE_CRITICAL(std::format("[Infinite loop error] Tried to assign a node a duplicate identifier. Tried (0), ..., ({})", dupeIndex - 1));
			}

			m_name = dupeName;
		}
	}


	KeyInput *Entity::keyInput() const { return m_keyInput.get(); }


	void Entity::startup()
	{
		if (!m_enabled) return;
		if (m_started) return;
		m_started = true;

		for (auto const &child : tree().m_children)
		{
			child->startup();
		}
	}


	void Entity::shutdown()
	{
		if (!m_enabled) return;
		if (!m_started) return;
		m_started = false;

		for (auto const &child : tree().m_children)
		{
			child->shutdown();
		}
	}


	void Entity::preupdate()
	{
		// Disown all children waiting to be disowned
		auto &tree = this->tree();
		for (Entity *toDisown : tree.m_childrenAwaitingDisown)
		{
			EventManager::invokeEvent<Entity *>("NodeDisowned", toDisown);
			tree.m_children.erase(tree.m_children.begin() + toDisown->tree().getSiblingIndex());
		}
		tree.m_childrenAwaitingDisown.clear();

		// Adopt all children waiting to be adopted
		for (auto it = tree.m_childrenAwaitingAdopt.begin(); it != tree.m_childrenAwaitingAdopt.end(); ++it)
		{
			std::unique_ptr<Entity> child = std::move(std::get<0>(*it));
			auto index = std::get<1>(*it);

			// Insert at `index` if provided, otherwise just push_back
			if (index.has_value())
			{
				TE_CORE_TRACE(std::format("{} insert child (index {})", name(), index.value()));
				tree.m_children.insert(tree.m_children.begin() + index.value(), std::move(child));
			}
			else
			{
				TE_CORE_TRACE(std::format("{} push_back child (index {})", name(), tree.m_children.size()));
				tree.m_children.push_back(std::move(child));
			}

			Entity *entity = tree.m_children.back().get();
			// setName sets name to NAME (0) if any siblings share its name (then NAME (1) if NAME (0) already exists, etc...)
			entity->setName(entity->m_name);
			EventManager::invokeEvent<Entity *>("EntityAdopted", entity);
		}
		tree.m_childrenAwaitingAdopt.clear();
	}


	void Entity::update()
	{
		if (!m_enabled) return;
		if (m_visible) draw();

		preupdate();

		// Handle scripts
		if (m_started)
		{
			for (const auto &script : m_scripts)
			{
				if (script->getEnabled())
				{
					script->update();
				}
			}
		}

		// Update KeyInput (decay inputs)
		if (m_keyInput) m_keyInput->update();

		// Recursively update
		for (auto it = m_children.begin(); it != m_children.end(); ++it)
		{
			(*it)->update();
		}
	}


	void Entity::destroy()
	{
		tree().getParent().tree().m_childrenAwaitingDisown.push_back(this);
	}


	void Node::addScript(std::unique_ptr<Script> script)
	{
		if (!m_keyInput)
		{
			// Create Editor KeyInput
			std::vector<int> registeredKeys = {
				// Function keys
				GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4, GLFW_KEY_F5, GLFW_KEY_F6,
				// Cam Movement keys
				GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_Q, GLFW_KEY_E,
				// Cam Rotation keys
				GLFW_KEY_I, GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L, GLFW_KEY_U, GLFW_KEY_O,
			};
			m_keyInput = std::make_unique<KeyInput>(registeredKeys);
		}

		m_scripts.push_back(std::move(script));
	}


	bool Node::removeScript(const Res &path)
	{
		auto it = std::find_if(m_scripts.begin(), m_scripts.end(), [&path](std::unique_ptr<Script> &ownedScript)
		{
			if (ownedScript.get()->getPath() == path)
			{
				return true;
			}
			return false;
		});

		if (it == m_scripts.end()) return false;
		m_scripts.erase(it);
		return true;
	}


	std::vector<Res> Node::getScriptPaths()
	{
		std::vector<Res> scriptPaths;
		for (const auto &script : m_scripts)
		{
			scriptPaths.push_back(script->getPath());
		}
		return scriptPaths;
	}


	void Node::update()
	{
		if (!m_enabled) return;
		if (m_visible) draw();

		preupdate();

		// Handle scripts
		if (m_started)
		{
			for (const auto &script : m_scripts)
			{
				if (script->getEnabled())
				{
					script->update();
				}
			}
		}

		// Update KeyInput (decay inputs)
		if (m_keyInput) m_keyInput->update();

		// Recursively update
		for (auto it = m_children.begin(); it != m_children.end(); ++it)
		{
			(*it)->update();
		}
	}


	TransformComponent &Entity::transform() const { return getComponent<TransformComponent>(); }


	TreeComponent &Entity::tree() const { return getComponent<TreeComponent>(); }
}