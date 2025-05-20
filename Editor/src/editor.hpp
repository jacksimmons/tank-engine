#pragma once
#include <imgui.h>
#include "engine.hpp"
#include "framebuffer.hpp"


namespace Tank
{
	class Node;
	class Scene;
	class KeyInput;

	namespace Editor
	{
		class EditorApp : public Application
		{
		private:
			std::unique_ptr<Node> m_initUI;
			/// <summary>
			/// Root node for system UI (Hierarchy, Inspector, etc.)
			/// </summary>
			std::unique_ptr<Node> m_system;
		public:
			EditorApp();
			~EditorApp();
			void initImGui();
			void loadScene(std::unique_ptr<Scene> scene);
			void loadDemoScene();
			void postSceneSetup();
			void drawMainMenuBar();
		protected:
			virtual void step() override;
			virtual void handleKeyInput() override;
		};
	}
}