#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include "application.hpp"
#include "key_input.hpp"
#include "log.hpp"
#include "static/time.hpp"
#include "nodes/node.hpp"
#include "nodes/scene.hpp"
#include "nodes/camera.hpp"
#include "nodes/light.hpp"
#include "nodes/sprite.hpp"
#include "nodes/model.hpp"
#include "nodes/cube_map.hpp"
#include "reflection/node_factory.hpp"


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
	Application::Application(const std::vector<int> &registeredKeys, ImGuiSettings settings)
	{
		// Init reflection
		m_nodeFactory = std::make_unique<Reflect::NodeFactory>();
		m_nodeFactory->registerClass<Node>("Node");
		m_nodeFactory->registerClass<Scene>("Scene");
		m_nodeFactory->registerClass<Camera>("Camera");
		m_nodeFactory->registerClass<DirLight>("DirLight");
		m_nodeFactory->registerClass<PointLight>("PointLight");

		m_windowSize = glm::ivec2(800, 600);
		m_settings.configFlags = settings.configFlags;
		m_settings.mainWinFlags = ImGuiWindowFlags_NoResize | settings.mainWinFlags;

		initGLFW();
		initGLAD();
		initImGui();
		m_context = ImGui::GetCurrentContext();
		m_keyInput = std::make_unique<KeyInput>(registeredKeys);
	}


	Application::~Application()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
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

		// Set GL version hint
		const int GL_MAJOR = 4;
		const int GL_MINOR = 6;
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
		KeyInput::setupKeyInputs(m_window);
		glfwSetFramebufferSizeCallback(m_window, Application::onWindowSizeChange);
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
		std::string graphicalDeviceInfo = (const char*)glGetString(GL_VENDOR)
			+ std::string(": ") + (const char*)glGetString(GL_RENDERER);
		std::string title = "TankEngine [" + graphicalDeviceInfo + "]";
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


	void Application::beginImGui(ImGuiIO &io)
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
		ImGuiIO &io = ImGui::GetIO();
		auto frameStart = std::clock();
		auto frameEnd = std::clock();
		float lastFrameDelta = 0;

		// ===== MAINLOOP =====
		while (!glfwWindowShouldClose(m_window))
		{
			frameStart = std::clock();

			glfwPollEvents();

			beginImGui(io);

			if (m_keyInput)
			{
				handleKeyInput();
				// Decay input states (comes after handleKeyInput)
				m_keyInput->update();
			}

			uiStep();

			endImGui();

			step();

			// Double buffering
			glfwSwapBuffers(m_window);

			frameEnd = std::clock();
			Time::setFrameDelta(frameStart, frameEnd);
		}
	}
}
