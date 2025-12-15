#pragma once
#include <imgui/imgui.h>


struct ImGuiSettings
{
	ImGuiConfigFlags configFlags;
	ImGuiWindowFlags mainWinFlags;
};


namespace Tank
{
	class KeyInput;
	class AudioEngine;
	namespace Reflect { class NodeFactory; }

	class TANK_API Application
	{
	private:
		ImGuiContext *m_context;
		glm::ivec2 m_windowSize;
		GLFWwindow *m_window;
		std::unique_ptr<AudioEngine> m_audioEngine;
	protected:
		std::unique_ptr<KeyInput> m_keyInput;
		std::unique_ptr<Reflect::NodeFactory> m_factory;
		ImGuiSettings m_settings;

	private:
		void initGLFW();
		void initGLAD();
		void initImGui();
	protected:
		virtual void handleKeyInput() = 0;
		void beginImGui(ImGuiIO &io);
		void endImGui();
		virtual void uiStep() {};
		virtual void step() {};

		Application(const std::vector<int> &registeredKeys, const ImGuiSettings settings = {
			0,
			0
		});
	public:
		virtual ~Application();

		/// <summary>
		/// Callback occurs when window size changes.
		/// </summary>
		static void onWindowSizeChange(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

		void run();

		const glm::ivec2 &getWindowSize() { return m_windowSize; }
		GLFWwindow *const getWindow() { return m_window; }
		ImGuiContext *const getContext() { return m_context; }
	};


	std::unique_ptr<Tank::Application> createApplication();
}
