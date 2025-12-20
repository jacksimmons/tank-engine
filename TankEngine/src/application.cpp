#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "application.hpp"
#include "key_input.hpp"
#include "log.hpp"
#include "events/event_manager.hpp"
#include "nodes/camera.hpp"
#include "nodes/cube_map.hpp"
#include "nodes/light.hpp"
#include "nodes/model.hpp"
#include "nodes/node.hpp"
#include "nodes/scene.hpp"
#include "nodes/sprite.hpp"
#include "nodes/audio.hpp"
#include "reflection/node_factory.hpp"
#include "static/time.hpp"
#include "utils/getset.hpp"


// Enable debug output
static void GLAPIENTRY msgCallback(GLenum source,
	GLenum type, GLenum id, GLenum severity, GLsizei length, const GLchar* message,
	const void* userParam)
{
	TE_CORE_ERROR(std::format("GL CALLBACK: {} type = {}, severity = {}, message = {}",
		type == GL_DEBUG_TYPE_ERROR ? "**GL ERROR**" : "",
		type, severity, message));
}


namespace Tank
{
	Application::Application(bool gui, ImGuiSettings settings)
		: m_gui(gui)
	{
		// Init reflection
		m_factory = std::make_unique<Reflect::NodeFactory>();
		m_factory->registerClass<Node>("Node");
		m_factory->registerClass<Scene>("Scene");
		m_factory->registerClass<Camera>("Camera");
		m_factory->registerClass<DirLight>("DirLight");
		m_factory->registerClass<PointLight>("PointLight");
		m_factory->registerClass<Sprite>("Sprite");
		m_factory->registerClass<Model>("Model");
		m_factory->registerClass<CubeMap>("CubeMap");
		m_factory->registerClass<Audio>("Audio");

		// Init events
		EventManager::addEvent("NodeAdopted", new Event<Node*>());
		EventManager::addEvent("NodeDisowned", new Event<Node*>());

		m_windowSize = glm::ivec2(800, 600);
		m_settings.configFlags = settings.configFlags;
		m_settings.mainWinFlags = ImGuiWindowFlags_NoResize | settings.mainWinFlags;

		initGLFW();
		initGLAD();
		if (m_gui)
		{
			initImGui();
			m_context = ImGui::GetCurrentContext();
		}
	}


	Application::~Application()
	{
		if (m_gui)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}


	void Application::initGLFW()
	{
		// Check GLFW initialises properly
		if (!glfwInit())
		{
			TE_CORE_CRITICAL("GLFW failed to initialise.");
			glfwTerminate();
		}

		// Set GL version hint.
		// @WARN: 4.2 causes crash with glGetString(GL_VENDOR) below.
		const int GL_MAJOR = 4;
		const int GL_MINOR = 3;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		// Create window, set current context
		m_window = glfwCreateWindow(m_windowSize.x, m_windowSize.y, (char *)"TankEngine", nullptr, nullptr);
		if (m_window == nullptr)
		{
			TE_CORE_CRITICAL(std::format("GLFW failed to create window. Does your machine support OpenGL version {}.{}?", GL_MAJOR, GL_MINOR));
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(m_window);

		// Initialise callbacks
		glfwSetFramebufferSizeCallback(m_window, Application::onWindowSizeChange);
		glfwSetKeyCallback(m_window, KeyInput::callback);
	}


	void Application::initGLAD()
	{
		// Initialise GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			TE_CORE_CRITICAL("GLAD failed to initialise.");
			return;
		}

		// Create viewport
		glViewport(0, 0, m_windowSize.x, m_windowSize.y);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		// Enable debug output
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(msgCallback, nullptr);
		// Re-set title to add graphics info
		std::string gpuInfo = (const char*)glGetString(GL_VENDOR)
			+ std::format("({})", (const char*)glGetString(GL_RENDERER));

		std::string title = "TankEngine " +
			std::format("[GPU: {}] ", gpuInfo) +
			std::format("[OpenGL: {}]", (const char *)glGetString(GL_VERSION));
		glfwSetWindowTitle(m_window, title.c_str());
	}


	void Application::initImGui()
	{
		// Initialise ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		io.ConfigFlags = m_settings.configFlags;

		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");
		ImGui::StyleColorsDark();
	}


	void Application::beginImGui(const ImGuiIO &io)
	{
		// Draw UI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 20));
		ImGui::SetNextWindowSize(io.DisplaySize);

		ImGui::Begin("##Main", nullptr, m_settings.mainWinFlags);
	}


	void Application::endImGui()
	{
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}


	void Application::run()
	{
		auto frameStart = std::clock();
		auto frameEnd = std::clock();
		float lastFrameDelta = 0;

		// =-=-=-= MAINLOOP =-=-=-=
		while (!glfwWindowShouldClose(m_window))
		{
			frameStart = std::clock();

			glfwPollEvents();

			if (m_gui) beginImGui(ImGui::GetIO());

			step();
			uiStep();

			if (m_gui) endImGui();

			// Double buffering
			glfwSwapBuffers(m_window);

			frameEnd = std::clock();
			Time::setFrameDelta(frameStart, frameEnd);
		}
	}
}
