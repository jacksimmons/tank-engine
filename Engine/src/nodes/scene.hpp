#pragma once
#include "nodes/node.hpp"


namespace Tank
{
	class Node;
	class Camera;
	class Light;
	class Shader;
	enum class LightType;

	namespace Editor { class _Hierarchy; }

	class TANK_API Scene : public Node, private ISerialisable
	{
		// The Hierarchy may modify elements of the scene (lights, nodes).
		friend class Editor::_Hierarchy;
	public:
		virtual json serialise();
		static void deserialise(const json &serialised, Scene **targetPtr);

		// Static
	private:
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
		Camera *m_activeCamera;
		std::vector<Light *> m_lights;

		void onNodeDeleted(Node *deleted) noexcept;
	public:
		// A Scene has ownership of the entire Node hierarchy, and a reference to
		// the active camera.
		Scene(const std::string &name = "Scene");
		
		// Get the active camera for this scene.
		Camera *getActiveCamera() const noexcept { return m_activeCamera; }
		// Set the active camera for this scene.
		void setActiveCamera(Camera *camera) noexcept { m_activeCamera = camera; }
		
		// Adds a light to the scene. Returns the light's index.
		unsigned addLight(Light *);
		// Removes a light to the scene.
		void removeLight(Light *);

		std::vector<Light *> getLights() const { return m_lights; }
		unsigned getNumLights(LightType type) const;
		
		virtual void update() override;
	};
}