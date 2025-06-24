#pragma once
#include <imgui.h>
#include "engine.hpp"


namespace Tank
{
	class Node;
	class Scene;
	class KeyInput;
	class _SceneView;

	class Player : public Application
	{
	private:
		std::unique_ptr<Node> m_root;
		std::unique_ptr<_SceneView> m_sceneView;
	public:
		Player();
		~Player();
		void loadScene(std::unique_ptr<Scene> scene);
	protected:
		virtual void step() override;
		virtual void handleKeyInput() override;
	};
}