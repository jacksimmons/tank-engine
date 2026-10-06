#pragma once
#include <entt/entt.hpp>
#include <serialisation/Serialisation.h>


namespace Tank
{
	class GameEntity;
	class GlobalLight;
	class Shader;
	struct CameraComponent;

	namespace Editor { class Hierarchy_; }


	class TANK_API Scene
	{
		friend class Entity;
		// The Hierarchy may modify elements of the scene (lights, nodes).
		friend class Editor::Hierarchy_;
	private:
		entt::registry m_registry;

		// Static
		static Scene *s_activeScene;
	public:
		static Scene *getActiveScene()
		{
			return s_activeScene;
		}
		static void setActiveScene(Scene *scene)
		{
			s_activeScene = scene;
		}

	// Instance
	private:
		CameraComponent *m_activeCamera;

		void onNodeDeleted(Entity *deleted) noexcept;
	public:
		Scene(bool isActive = false);

		std::unique_ptr<GameEntity> createEntity();

		// Get the active camera for this scene.
		CameraComponent *getActiveCamera() const noexcept { return m_activeCamera; }
		// Set the active camera for this scene.
		void setActiveCamera(CameraComponent *camera) noexcept { m_activeCamera = camera; }

		void update();
		void applyLightsToShader(const Shader &shader);
	};


	template <>
	json serialise<Scene>(Scene *);
	template <>
	Scene deserialise(const json &);
}