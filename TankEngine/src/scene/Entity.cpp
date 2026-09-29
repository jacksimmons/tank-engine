#include "Entity.h"
#include <components/Transform.h>
#include <components/Tree.h>
#include <events/EventManager.h>
#include <KeyInput.h>


namespace Tank
{
	Entity::Entity(const entt::entity handle, Scene *ecs, const std::string &name)
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


	TransformComponent &Entity::transform() const { return getComponent<TransformComponent>(); }


	TreeComponent &Entity::tree() const { return getComponent<TreeComponent>(); }


	// =======================
	//		Serialisation
	// =======================
	template <>
	json serialise(Entity *in)
	{
		json serialised;
		serialised["name"] = in->name();
		serialised["enabled"] = in->isEnabled();
		serialised["visible"] = in->isVisible();
		serialised["transform"] = serialise(&in->getComponent<TransformComponent>());
		return serialised;
	}

	template <>
	void deserialise(const json &serialised, Entity *out)
	{
		out->setName(serialised["name"]);
		out->setEnabled(serialised["enabled"]);
		out->setVisible(serialised["visible"]);
		deserialise(serialised["transform"], &out->getComponent<TransformComponent>());
	}
}