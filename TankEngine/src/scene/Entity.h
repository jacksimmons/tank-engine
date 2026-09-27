#pragma once
#include <entt/entt.hpp>
#include <Log.h>
#include "Scene.h"


namespace Tank
{
	class TANK_API Entity
	{
	public:
		Entity() = default;
		Entity(const entt::entity handle, Scene *scene);
		Entity(const Entity &other) = default;

		template <typename T>
		bool hasComponent()
		{
			return m_scene->m_registry.all_of<T>(m_handle);
		}

		template <typename T, typename... ComponentArgs>
		T &addComponent(ComponentArgs&&... args)
		{
			TE_CORE_ASSERT(!hasComponent<T>(), "Entity already has this component!");
			return m_scene->m_registry.emplace<T>(m_handle, std::forward<ComponentArgs>(args)...);
		}

		template <typename T>
		T &getComponent()
		{
			TE_CORE_ASSERT(hasComponent<T>(), "Entity doesn't have this component!");
			return m_scene->m_registry.get<T>(m_handle);
		}

		template <typename T>
		void removeComponent()
		{
			TE_CORE_ASSERT(hasComponent<T>(), "Entity doesn't have this component!");
			m_scene->m_registry.remove<T>(m_handle);
		}

		operator bool() const { return m_handle != entt::null; }
	private:
		entt::entity m_handle = entt::null;
		Scene *m_scene = nullptr;
	};
}