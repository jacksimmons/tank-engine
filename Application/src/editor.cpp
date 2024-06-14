#include <string>
#include <cmath>
#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "editor.hpp"
#include "key_input.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "framebuffer.hpp"
#include "transform.hpp"
#include "nodes/node.hpp"
#include "nodes/model.hpp"
#include "nodes/hierarchy.hpp"
#include "nodes/scene_view.hpp"
#include "nodes/inspector.hpp"
#include "nodes/light.hpp"


Editor::Editor()
{
	m_settings.configFlags = ImGuiConfigFlags_DockingEnable;
	m_settings.mainWinFlags = ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_NoBringToFrontOnFocus;
	m_settings.windowSize = glm::ivec2(800, 600);

	initGL();
	initImGui();

	// Flip loaded textures on the y-axis.
	stbi_set_flip_vertically_on_load(GL_TRUE);
	generateSceneThenInitInput();
}

void Editor::initGL()
{
	if (!glfwInit())
	{
		TE_CORE_CRITICAL("GLFW failed to initialise.");
		glfwTerminate();
	}

	// Set GL version hint
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	m_window = glfwCreateWindow(m_settings.windowSize.x, m_settings.windowSize.y, (char *)"TankEngine", nullptr, nullptr);
	if (m_window == nullptr)
	{
		TE_CORE_CRITICAL("GLFW failed to create window.");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_window);

	// Initialise callbacks
	KeyInput::setupKeyInputs(m_window);
	glfwSetFramebufferSizeCallback(m_window, Editor::onWindowSizeChange);

	// Initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		TE_CORE_CRITICAL("GLAD failed to initialise.");
		return;
	}

	// Create viewport
	glViewport(0, 0, m_settings.windowSize.x, m_settings.windowSize.y);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);
}

void Editor::initImGui()
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

void Editor::generateSceneThenInitInput()
{
	// Create scene (scopes help control ownership)
	auto root = std::make_unique<Tank::Node>("Root");
	{
		auto cam = std::make_unique<Tank::Camera>("Camera");
		root->addChild(std::move(cam));
	
		auto cube = std::make_unique<Tank::Model>("Cube", "shader.vert", "shader.frag");
		cube->addTexture("wall.jpg", GL_RGB, "tex0");
		cube->addTexture("awesomeface.png", GL_RGBA, "tex1");
		root->addChild(std::move(cube));
		
		auto light = std::make_unique<Tank::Light>("Light", glm::vec3(1, 0, 0));
		root->addChild(std::move(light));
	}

	m_uiRoot = std::make_unique<Tank::Node>("System");
	{
		auto inspector = std::make_unique<Tank::Inspector>("Inspector");
		m_uiRoot->addChild(std::move(inspector));
		m_uiRoot->addChild(std::make_unique<Tank::Hierarchy>("Hierarchy"));
		m_uiRoot->addChild(std::make_unique<Tank::SceneView>("SceneView", m_settings.windowSize, m_settings.windowSize));
	}
	
	Tank::Camera *cam = dynamic_cast<Tank::Camera *>(root->getChild("Camera"));

	auto scene = std::make_unique<Tank::Scene>(std::move(root), cam);
	Tank::Scene::setActiveScene(scene.get());
	m_scene = std::move(scene);

	// Initialise input. Requires scene init first.
	m_keyInput = std::make_unique<KeyInput>(std::vector<int>(
	{
		GLFW_KEY_ESCAPE,
		GLFW_KEY_F1,

		GLFW_KEY_W,
		GLFW_KEY_A,
		GLFW_KEY_S,
		GLFW_KEY_D,
		GLFW_KEY_Q,
		GLFW_KEY_E,

		GLFW_KEY_I,
		GLFW_KEY_J,
		GLFW_KEY_K,
		GLFW_KEY_L,
		GLFW_KEY_U,
		GLFW_KEY_O
	}));
}

void Editor::run()
{	
	ImGuiIO &io = ImGui::GetIO();

	// ===== MAINLOOP =====
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw UI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0,0));
		ImGui::SetNextWindowSize(io.DisplaySize);
		
		ImGui::Begin("##Main", nullptr, m_settings.mainWinFlags);

		//ImGui::ShowDemoWindow();

		handleKeyInput();
		// Decay input states (comes after handleKeyInput)
		m_keyInput->update();
		m_uiRoot->update();

		ImGui::End();
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//// ! Remove this?
		GLFWwindow *backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
		// ! End

		// Double buffering
		glfwSwapBuffers(m_window);
	}
}

void Editor::handleKeyInput()
{
	
	if (m_keyInput->getKeyState(GLFW_KEY_ESCAPE) == KeyState::Pressed)
		glfwSetWindowShouldClose(m_window, GL_TRUE);

	if (m_keyInput->getKeyState(GLFW_KEY_F1) == KeyState::Pressed)
		m_keyInput->cycleRenderMode();

	auto cam = Tank::Scene::getActiveScene()->getActiveCamera();
	if (cam == nullptr) return;

	if (m_keyInput->getKeyState(GLFW_KEY_W) == KeyState::Held)
		cam->translate(glm::vec3(0.0f, 0.01f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_A) == KeyState::Held)
		cam->translate(glm::vec3(-0.01f, 0.0f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_S) == KeyState::Held)
		cam->translate(glm::vec3(0.0f, -0.01f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_D) == KeyState::Held)
		cam->translate(glm::vec3(0.01f, 0.0f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_Q) == KeyState::Held)
		cam->translate(glm::vec3(0.0f, 0.0f, 0.01f));

	if (m_keyInput->getKeyState(GLFW_KEY_E) == KeyState::Held)
		cam->translate(glm::vec3(0.0f, 0.0f, -0.01f));


	if (m_keyInput->getKeyState(GLFW_KEY_J) == KeyState::Held)
		cam->rotate(glm::vec3(-0.01f, 0.0f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_L) == KeyState::Held)
		cam->rotate(glm::vec3(0.01f, 0.0f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_I) == KeyState::Held)
		cam->rotate(glm::vec3(0.0f, 0.01f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_K) == KeyState::Held)
		cam->rotate(glm::vec3(0.0f, -0.01f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_U) == KeyState::Held)
		cam->rotate(glm::vec3(0.0f, 0.0f, 0.01f));

	if (m_keyInput->getKeyState(GLFW_KEY_O) == KeyState::Held)
		cam->rotate(glm::vec3(0.0f, 0.0f, -0.01f));
}

Editor::~Editor()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

std::unique_ptr<Tank::Application> Tank::createApplication()
{
	return std::make_unique<Editor>();
}