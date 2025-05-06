#pragma once

#include <GLFW/glfw3.h>
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
			struct WindowSettings
			{
				ImGuiConfigFlags configFlags;
				ImGuiWindowFlags mainWinFlags;
				glm::ivec2 windowSize;
			};

			GLFWwindow *m_window;

			std::unique_ptr<Node> m_initUI;
			/// <summary>
			/// Root node for system UI (Hierarchy, Inspector, etc.)
			/// </summary>
			std::unique_ptr<Node> m_system;
			std::unique_ptr<KeyInput> m_keyInput;

			std::unique_ptr<WindowSettings> m_settings;
		public:
			/// <summary>
			/// Callback occurs when window size changes.
			/// </summary>
			static void onWindowSizeChange(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

			EditorApp();
			~EditorApp();
			void initGL();
			void initImGui();
			void loadScene(std::unique_ptr<Scene> scene);
			void loadDemoScene();
			void postSceneSetup();
			void run() override;
			void handleKeyInput();

			void drawMainMenuBar();

			WindowSettings *getWindowSettings() const noexcept { return m_settings.get(); }
		};
	}
}