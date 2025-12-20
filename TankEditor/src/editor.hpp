#pragma once
#include <imgui.h>
#include "engine.hpp"
#include "framebuffer.hpp"


class ImGuiIO;
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
			static std::vector<std::string> s_windowNames;
			
			std::unique_ptr<Node> m_initUI;
			/// <summary>
			/// Root node for system UI (Hierarchy, Inspector, etc.)
			/// </summary>
			std::unique_ptr<Node> m_system;
			
			// Keyboard input for the Editor only.
			std::unique_ptr<KeyInput> m_editorInput;
		public:
			EditorApp();
			~EditorApp();
			void initImGui();
			void loadScene(std::unique_ptr<Scene> scene);
			void loadDemoScene();
			void postSceneSetup();
		protected:
			virtual void step() override;
			virtual void uiStep() override;
			virtual void handleKeyInput();
			virtual void beginImGui(ImGuiIO &) override;
			virtual void endImGui() override;
		};
	}
}