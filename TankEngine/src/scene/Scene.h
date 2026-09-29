#pragma once
#include <entt/entt.hpp>
#include <serialisation/Serialisation.h>


namespace Tank
{
	class Entity;
	struct CameraComponent;
	struct LightComponent;
	class Shader;
	enum class LightType;

	namespace Editor { class Hierarchy_; }


	class TANK_API Scene
	{
		friend class Entity;
		// The Hierarchy may modify elements of the scene (lights, nodes).
		friend class Editor::Hierarchy_;
	public:
		Scene();
		~Scene();

		Entity createEntity();
		void update();
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
		std::vector<LightComponent *> m_lights;

		void onNodeDeleted(Entity *deleted) noexcept;
	public:
		// A Scene has ownership of the entire Node hierarchy, and a reference to
		// the active camera.
		Scene(const std::string &name = "Scene");

		// Get the active camera for this scene.
		CameraComponent *getActiveCamera() const noexcept { return m_activeCamera; }
		// Set the active camera for this scene.
		void setActiveCamera(CameraComponent *camera) noexcept { m_activeCamera = camera; }

		// Adds a light to the scene. Returns the light's index.
		unsigned addLight(LightComponent *);
		// Removes a light to the scene.
		void removeLight(LightComponent *);

		std::vector<LightComponent *> getLights() const { return m_lights; }
		unsigned getNumLights(LightType type) const;

		void update();
	};


	template <>
	json serialise<Scene>(Scene *);
	template <>
	void deserialise<Scene>(const json &, Scene *);
}