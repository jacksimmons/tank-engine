#pragma once
#include "tepch.hpp"
#include "core.hpp"


struct ImGuiSettings
{
	ImGuiConfigFlags configFlags;
	ImGuiWindowFlags mainWinFlags;
};


namespace Tank
{
	class KeyInput;
	class TANK_API Application
	{
	private:
		ImGuiSettings m_settings;
		ImGuiContext *m_context;
		glm::ivec2 m_windowSize;
		GLFWwindow *m_window;
	protected:
		std::unique_ptr<KeyInput> m_keyInput;

	private:
		void initGLFW();
		void initGLAD();
		void initImGui();
	protected:
		virtual void handleKeyInput() = 0;
		virtual void step() = 0;

		Application(const std::vector<int> &registeredKeys);
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
