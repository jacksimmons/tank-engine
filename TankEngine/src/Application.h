#pragma once
#include <imgui/imgui.h>


struct ImGuiSettings
{
	ImGuiConfigFlags configFlags;
	ImGuiWindowFlags mainWinFlags;
};
struct GLFWwindow;


namespace Tank
{
	class KeyInput;


	class TANK_API Application
	{
	private:
		ImGuiContext *m_context;
	protected:
		bool m_gui;
		glm::ivec2 m_windowSize;
		GLFWwindow *m_window;
		ImGuiSettings m_settings;

	private:
		void initGLFW();
		void initGLAD();
		void initImGui();
	protected:
		void beginImGui(const ImGuiIO &io);
		void endImGui();
		virtual void start() {};
		virtual void uiStep() {};
		virtual void step() {};

		Application(
			bool gui = false,
			ImGuiSettings settings = {
				0,
				0
			}
		);
	public:
		virtual ~Application();

		void run();

		const glm::ivec2 &getWindowSize() { return m_windowSize; }
		GLFWwindow *const getWindow() { return m_window; }
		ImGuiContext *const getContext() { return m_context; }
	};


	std::unique_ptr<Tank::Application> createApplication();
}
