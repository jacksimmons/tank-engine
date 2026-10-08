#pragma once
#include <entt/entt.hpp>
#include <Log.h>
#include "Scene.h"


namespace Tank
{
	/// @brief Wrapper for ECS entities.
	/// 
	/// ECS properties are kept private.
	/// 
	/// Only subclasses and friends can construct one.
	class TANK_API Entity
	{
		friend class Scene;
		friend class SceneSerialisation;
		friend class Physics;
	private:
		entt::entity m_handle = entt::null;
		Scene *m_ecs = nullptr;
	protected:
		/// <summary>
		/// If false, `update` isn't invoked for this entity and all children.
		/// </summary>
		bool m_enabled = true;

		/// <summary>
		/// If false, `draw` isn't invoked for this entity.
		/// </summary>
		bool m_visible = true;

		/// <summary>
		/// If true, this is controlled by the editor: the Hierarchy and Inspector are
		/// unable to edit this node.
		/// </summary>
		bool m_isEditorControlled = false;
	protected:
		Entity(entt::entity handle, Scene *ecs)
			: m_handle(handle), m_ecs(ecs) {}
		Entity(const Entity &other) = default;
	public:
		virtual ~Entity() = default;

		bool isEnabled() const noexcept { return m_enabled; }
		void setEnabled(bool enabled) noexcept { m_enabled = enabled; }

		bool isVisible() const noexcept { return m_visible; }
		void setVisible(bool visible) noexcept { m_visible = visible; }

		bool isEditorControlled() const noexcept { return m_isEditorControlled; }
		void setEditorControlled(bool editorControlled) noexcept { m_isEditorControlled = editorControlled; }

		template <typename T>
		bool hasComponent() const
		{
			return m_ecs->m_registry.all_of<T>(m_handle);
		}

		template <typename T, typename... ComponentArgs>
		T &addComponent(ComponentArgs&&... args)
		{
			TE_CORE_ASSERT(!hasComponent<T>(), "Entity already has this component!");
			return m_ecs->m_registry.emplace<T>(m_handle, std::forward<ComponentArgs>(args)...);
		}

		template <typename T>
		T &getComponent() const
		{
			TE_CORE_ASSERT(hasComponent<T>(), "Entity doesn't have this component!");
			return m_ecs->m_registry.get<T>(m_handle);
		}

		template <typename T>
		void removeComponent()
		{
			TE_CORE_ASSERT(hasComponent<T>(), "Entity doesn't have this component!");
			m_ecs->m_registry.remove<T>(m_handle);
		}

		operator bool() const { return m_handle != entt::null; }
	};
}