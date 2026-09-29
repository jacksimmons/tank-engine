#pragma once
#include <entt/entt.hpp>
#include <Log.h>
#include "Scene.h"


namespace Tank
{
	class TransformComponent;
	class TreeComponent;
	class KeyInput;


	/// @brief The most basic game object which is registered by the ECS.
	class TANK_API Entity
	{
	private:
		entt::entity m_handle = entt::null;
		Scene *m_ecs = nullptr;

		std::string m_name;

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
	public:
		Entity() = default;
		Entity(const entt::entity handle, Scene *ecs, const std::string &name = "");
		Entity(const Entity &other) = default;
		virtual ~Entity() = default;

		const std::string &name() const noexcept { return m_name; }
		void setName(const std::string &name) noexcept;

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
		
		
		// Common components
		TransformComponent &transform() const;
		TreeComponent &tree() const;


		virtual void startup() {};
		virtual void shutdown() {};
		virtual void preupdate();
		virtual void update();
		virtual void destroy();
	};


	template <>
	json serialise<Entity>(Entity *);
	template <>
	void deserialise<Entity>(const json &, Entity *);
}