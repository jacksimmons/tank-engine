#include <GLFW/glfw3.h>
#include <KeyInput.h>
#include <components/Name.h>
#include <components/Transform.h>
#include <scene/Scene.h>
#include <scripting/Script.h>
#include <events/EventManager.h>
#include "GameEntity.h"
#include "Entity.h"


namespace Tank
{
	const std::string &GameEntity::name() const { return getComponent<NameComponent>().Name; }

	void GameEntity::setName(const std::string &name) noexcept
	{
		auto &nc = getComponent<NameComponent>();
		nc.Name = name;

		// Get a vector of all sibling names
		std::vector<std::string> siblingNames;
		auto &transform = this->transform();
		for (const auto *sibling : transform.getSiblings())
		{
			if (sibling == this) continue;
			siblingNames.push_back(sibling->name());
		}

		std::vector<GameEntity *> siblings = transform.getSiblings();
		std::function<GameEntity *()> findSiblingWithSameName = [this, &siblings]()
		{
			auto it = std::find_if(siblings.begin(), siblings.end(),
				[this](const GameEntity *sibling)
			{
				return this != sibling && sibling->name() == this->name();
			});

			if (it != siblings.end()) return *it;
			return (GameEntity *)nullptr;
		};

		// If a sibling already uses this name, add a (1) [or (2) or (3) ... if necessary]
		if (findSiblingWithSameName() != nullptr)
		{
			int dupeIndex;
			std::string baseName = this->name();
			std::string dupeName;

			// Try each of NAME (1), NAME (2), ... NAME (NUM_SIBLINGS - 1)
			for (dupeIndex = 0; dupeIndex < this->transform().getSiblingCount(); dupeIndex++)
			{
				dupeName = std::format("{} ({})", baseName, dupeIndex);
				nc.Name = dupeName;
				if (findSiblingWithSameName() == nullptr) break;
			}

			// If even NAME (NUM_SIBLINGS - 1) didn't work, we have a problem
			if (dupeIndex == this->transform().getSiblingCount())
			{
				TE_CORE_FATAL(std::format("[Infinite loop error] Tried to assign a node a duplicate identifier. Tried (0), ..., ({})", dupeIndex - 1));
			}

			nc.Name = dupeName;
		}
	}


	KeyInput *GameEntity::keyInput() const { return m_keyInput.get(); }

	TransformComponent &GameEntity::transform() const { return getComponent<TransformComponent>(); }


	void GameEntity::startup()
	{
		if (!isEnabled()) return;
		if (m_started) return;
		m_started = true;

		for (auto const &child : transform().m_children)
		{
			child->startup();
		}
	}


	void GameEntity::shutdown()
	{
		if (!isEnabled()) return;
		if (!m_started) return;
		m_started = false;

		for (auto const &child : transform().m_children)
		{
			child->shutdown();
		}
	}


	void GameEntity::addScript(std::unique_ptr<Script> script)
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


	bool GameEntity::removeScript(const Res &path)
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


	std::vector<Res> GameEntity::getScriptPaths() const
	{
		std::vector<Res> scriptPaths;
		for (const auto &script : m_scripts)
		{
			scriptPaths.push_back(script->getPath());
		}
		return scriptPaths;
	}


	void GameEntity::preupdate()
	{
		// Disown all children waiting to be disowned
		auto &tree = this->transform();
		for (GameEntity *toDisown : tree.m_childrenAwaitingDisown)
		{
			EventManager::invokeEvent<GameEntity *>("EntityDisowned", toDisown);
			tree.m_children.erase(tree.m_children.begin() + toDisown->transform().getSiblingIndex());
		}
		tree.m_childrenAwaitingDisown.clear();

		// Adopt all children waiting to be adopted
		for (auto it = tree.m_childrenAwaitingAdopt.begin(); it != tree.m_childrenAwaitingAdopt.end(); ++it)
		{
			std::unique_ptr<GameEntity> child = std::move(std::get<0>(*it));
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

			GameEntity *entity = tree.m_children.back().get();
			// setName sets name to NAME (0) if any siblings share its name (then NAME (1) if NAME (0) already exists, etc...)
			entity->setName(entity->name());
			EventManager::invokeEvent<Entity *>("EntityAdopted", entity);
		}
		tree.m_childrenAwaitingAdopt.clear();
	}


	void GameEntity::update()
	{
		if (!isEnabled()) return;

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
		for (auto it = transform().m_children.begin(); it != transform().m_children.end(); ++it)
		{
			(*it)->update();
		}
	}


	void GameEntity::destroy()
	{
		transform().getParent()->transform().m_childrenAwaitingDisown.push_back(this);
	}
}