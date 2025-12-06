#include <format>
#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <events/event_manager.hpp>
#include "key_input.hpp"
#include "static/time.hpp"
#include "nodes/scene.hpp"
#include "nodes/camera.hpp"
#include "ui/scene_view.hpp"
#include <colours.hpp>


namespace Tank::Editor
{
	const WindowOpts WINDOW_OPTS = {
		ImGuiWindowFlags_None,
		false,
		false
	};


	_SceneView::_SceneView(const std::string &name, glm::ivec2 sceneViewportSize, glm::ivec2 fbViewportSize, KeyInput *keyInput, bool isPlayer)
		: _Window(name, WINDOW_OPTS), m_isPlayer(isPlayer)
	{
		m_sceneW = sceneViewportSize.x, m_sceneH = sceneViewportSize.y;
		m_fb = std::make_unique<Framebuffer>(fbViewportSize.x, fbViewportSize.y);
		m_keyInput = keyInput;
		m_isFocussed = false;
		m_isInPlayMode = false;

		// Explicitly setup gl with default modes
		m_polygonMode = GL_FILL;
		glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);

		m_cullFaceMode = GL_BACK;
		glEnable(GL_CULL_FACE);
		glCullFace(m_cullFaceMode);
		
		m_frontFaceMode = GL_CCW;
		glFrontFace(m_frontFaceMode);

		m_depthFuncComparisonMode = GL_LESS;
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(m_depthFuncComparisonMode);

		m_blendFuncSFactor = GL_SRC_ALPHA;
		m_blendFuncDFactor = GL_ONE_MINUS_SRC_ALPHA;
		glEnable(GL_BLEND);
		glBlendFunc(m_blendFuncSFactor, m_blendFuncDFactor);
	}

	void _SceneView::draw()
	{
		int fbW = m_fb->getW(), fbH = m_fb->getH();

		// Just sets default panel-window size.
		ImGui::SetNextWindowSize(ImVec2(fbW + 10.0f, fbH + 10.0f), ImGuiCond_FirstUseEver);
		_Window::draw();
	}

	void _SceneView::drawPanel()
	{
		int fbW = m_fb->getW(), fbH = m_fb->getH();

		// If this window is the Player, then render to the root ImGui window.
		if (!m_isPlayer) ImGui::BeginChild("SceneRender");
		m_isFocussed = ImGui::IsWindowFocused();

		ImVec2 wsize = ImGui::GetWindowSize();
		int fbWNew = (int)wsize.x - 10;
		int fbHNew = (int)wsize.y - 10;

		if (fbWNew != fbW || fbHNew != fbH)
			rescale(fbWNew, fbHNew);

		ImVec2 fbsize = ImVec2((float)fbW, (float)fbH);
		ImTextureID imTex = (ImTextureID)(intptr_t)m_fb->getTexColBuf();

		// Play mode button (once clicked, hide the button and replace it with Stop)
		// Only display "Play" if not in Play Mode.
		if (m_isPlayer || m_isInPlayMode || ImGui::Button("Play"))
		{
			m_isInPlayMode = true;
			// Enable all scripting (and disable all EditorNodes)
			Scene *activeScene = Scene::getActiveScene();
			activeScene->startup();
			if (m_parent) m_parent->startup();

			// Only display "Stop", and allow its use, if not always in Play Mode.
			if (!m_isPlayer && ImGui::Button("Stop"))
			{
				m_isInPlayMode = false;

				// Disable all scripting (and enable all EditorNodes)
				Scene *activeScene = Scene::getActiveScene();
				//activeScene->getActiveCamera()->setFreeLook(true);
				activeScene->shutdown();
				if (m_parent) m_parent->shutdown();
			}
		}

		// Update FPS counter at a constant frequency. If not updated,
		// display previous FPS value.
		float delta = Time::getFrameDelta();
		m_fpsDisplayUpdateTimer += delta;
		if (m_fpsDisplayUpdateTimer > FPS_DISPLAY_UPDATE_FREQUENCY)
		{
			m_fpsDisplayLastText = std::format("{} FPS", 1 / delta).c_str();
			m_fpsDisplayUpdateTimer = 0;
		}
		ImGui::SameLine();
		ImGui::Text(m_fpsDisplayLastText.c_str());

		ImGui::Image(imTex, fbsize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		if (!m_isPlayer) ImGui::EndChild();
	}

	void _SceneView::startup()
	{
		// Ignore EditorNode disabling behaviour
	}

	void _SceneView::update()
	{
		int fbW = m_fb->getW(), fbH = m_fb->getH();

		// Set viewport to fbo, render into fbo, set viewport to std.
		glViewport(0, 0, fbW, fbH);
		m_fb->update();
		glViewport(0, 0, m_sceneW, m_sceneH);

		_Window::update();
	}

	void _SceneView::shutdown()
	{
		// Ignore EditorNode enabling behaviour
	}

	void _SceneView::rescale(int w, int h) const
	{
		// Re-render the framebuffer at new scale when the user has finished scaling.
		if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			m_fb->rescale(w, h);
		}
	}

	void _SceneView::handleKeyInput()
	{
		if (!m_isFocussed) return;

		auto cam = Scene::getActiveScene()->getActiveCamera();
		if (cam == nullptr) return;
		float panSpd = cam->getPanSpeed();
		float rotSpd = cam->getRotSpeed();

		if (m_keyInput->getKeyState(GLFW_KEY_F1) == KeyState::Pressed)
			cyclePolygonMode();
		if (m_keyInput->getKeyState(GLFW_KEY_F2) == KeyState::Pressed)
			cycleCullFaceMode();
		if (m_keyInput->getKeyState(GLFW_KEY_F3) == KeyState::Pressed)
			cycleFrontFaceMode();
		if (m_keyInput->getKeyState(GLFW_KEY_F4) == KeyState::Pressed)
			cycleDepthFuncComparisonMode();
		if (m_keyInput->getKeyState(GLFW_KEY_F5) == KeyState::Pressed)
			cycleBlendFuncFactor(m_blendFuncSFactor, "sfactor");
		if (m_keyInput->getKeyState(GLFW_KEY_F6) == KeyState::Pressed)
			cycleBlendFuncFactor(m_blendFuncDFactor, "dfactor");

		float frameDelta = Time::getFrameDelta();

		// Exit early if camera free look is disabled.
		if (!cam->getFreeLook()) return;

		if (m_keyInput->getKeyState(GLFW_KEY_W) == KeyState::Held)
			cam->translate(glm::vec3(0.0f, frameDelta * panSpd, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_A) == KeyState::Held)
			cam->translate(glm::vec3(-frameDelta * panSpd, 0.0f, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_S) == KeyState::Held)
			cam->translate(glm::vec3(0.0f, -frameDelta * panSpd, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_D) == KeyState::Held)
			cam->translate(glm::vec3(frameDelta * panSpd, 0.0f, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_Q) == KeyState::Held)
			cam->translate(glm::vec3(0.0f, 0.0f, frameDelta * panSpd));

		if (m_keyInput->getKeyState(GLFW_KEY_E) == KeyState::Held)
			cam->translate(glm::vec3(0.0f, 0.0f, -frameDelta * panSpd));


		if (m_keyInput->getKeyState(GLFW_KEY_J) == KeyState::Held)
			cam->rotate(glm::vec3(-frameDelta * rotSpd, 0.0f, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_L) == KeyState::Held)
			cam->rotate(glm::vec3(frameDelta * rotSpd, 0.0f, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_I) == KeyState::Held)
			cam->rotate(glm::vec3(0.0f, frameDelta * rotSpd, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_K) == KeyState::Held)
			cam->rotate(glm::vec3(0.0f, -frameDelta * rotSpd, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_U) == KeyState::Held)
			cam->rotate(glm::vec3(0.0f, 0.0f, frameDelta * rotSpd));

		if (m_keyInput->getKeyState(GLFW_KEY_O) == KeyState::Held)
			cam->rotate(glm::vec3(0.0f, 0.0f, -frameDelta * rotSpd));
	}

	void _SceneView::cyclePolygonMode()
	{
		switch (m_polygonMode)
		{
		case GL_FILL:
			m_polygonMode = GL_POINT;
			break;
		case GL_POINT:
			m_polygonMode = GL_LINE;
			break;
		case GL_LINE:
			m_polygonMode = GL_FILL;
			break;
		}

		glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);
		std::string line = std::format("Set GL polygon mode to {}",
			m_polygonMode == GL_FILL ? "FILL" : (m_polygonMode == GL_POINT ? "POINT" : "LINE"));

		EventManager::invokeEvent("Console.AddColouredLine", line, Tank::Colour::INFO);
	}

	void _SceneView::cycleCullFaceMode()
	{
		switch (m_cullFaceMode)
		{
		case GL_BACK:
			m_cullFaceMode = GL_FRONT;
			break;
		case GL_FRONT:
			m_cullFaceMode = GL_FRONT_AND_BACK;
			break;
		case GL_FRONT_AND_BACK:
			m_cullFaceMode = GL_BACK;
			break;
		}

		glCullFace(m_cullFaceMode);

		std::string line = std::format("Set GL cull face mode to {}",
			m_cullFaceMode == GL_BACK ? "BACK" : (m_cullFaceMode == GL_FRONT ? "FRONT" : "FRONT_AND_BACK"));

		EventManager::invokeEvent("Console.AddColouredLine", line, Tank::Colour::INFO);
	}

	void _SceneView::cycleFrontFaceMode()
	{
		switch (m_frontFaceMode)
		{
		case GL_CCW:
			m_frontFaceMode = GL_CW;
			break;
		case GL_CW:
			m_frontFaceMode = GL_CCW;
			break;
		}

		glFrontFace(m_frontFaceMode);

		std::string line = std::format("Set GL front face mode (winding order) to {}",
			m_frontFaceMode == GL_CCW ? "CCW" : "CW");

		EventManager::invokeEvent("Console.AddColouredLine", line, Tank::Colour::INFO);
	}

	void _SceneView::cycleDepthFuncComparisonMode()
	{
		std::string newMode = "(Null)";
		switch (m_depthFuncComparisonMode)
		{
		// Loop on ALWAYS back to NEVER
		case GL_ALWAYS:
			m_depthFuncComparisonMode = GL_NEVER;
			newMode = "NEVER";
			break;
		default:
			m_depthFuncComparisonMode++;
			newMode =
				m_depthFuncComparisonMode == GL_LESS ? "LESS (<)"
				: m_depthFuncComparisonMode == GL_EQUAL ? "EQUAL (==)"
				: m_depthFuncComparisonMode == GL_LEQUAL ? "LEQUAL (<=)"
				: m_depthFuncComparisonMode == GL_GREATER ? "GREATER (>)"
				: m_depthFuncComparisonMode == GL_NOTEQUAL ? "NOTEQUAL (!=)"
				: m_depthFuncComparisonMode == GL_GEQUAL ? "GEQUAL (>=)"
				: m_depthFuncComparisonMode == GL_ALWAYS ? "ALWAYS"
				: "(Invalid)";
			break;
		}

		glDepthFunc(m_depthFuncComparisonMode);

		std::string line = "Set GL depth func comparison to " + newMode;
		EventManager::invokeEvent("Console.AddColouredLine", line, Tank::Colour::INFO);
	}

	
	void _SceneView::cycleBlendFuncFactor(GLenum &factor, const std::string &name)
	{
		std::map<GLenum, std::string> enumToName = {
			{ GL_ZERO, "ZERO (0,0,0,0)" },
			{ GL_ONE, "ONE (1,1,1,1)" },
			{ GL_SRC_COLOR, "SRC_COLOR" },
			{ GL_ONE_MINUS_SRC_COLOR, "1 - SRC_COLOR" },
			{ GL_DST_COLOR, "DST_COLOR" },
			{ GL_ONE_MINUS_DST_COLOR, "1 - DST_COLOR" },
			{ GL_SRC_ALPHA, "SRC_ALPHA" },
			{ GL_ONE_MINUS_SRC_ALPHA, "1 - SRC_ALPHA" },
			{ GL_CONSTANT_COLOR, "CONSTANT_COLOR" },
			{ GL_ONE_MINUS_CONSTANT_COLOR, "1 - CONSTANT_COLOR" },
			{ GL_CONSTANT_ALPHA, "CONSTANT_ALPHA" },
			{ GL_ONE_MINUS_CONSTANT_ALPHA, "1 - CONSTANT_ALPHA" },
			{ GL_SRC_ALPHA_SATURATE, "SRC_ALPHA_SATURATE" },
			{ GL_SRC1_COLOR, "SRC1_COLOR" },
			{ GL_ONE_MINUS_SRC1_COLOR, "1 - SRC1_COLOR" },
			{ GL_SRC1_ALPHA, "SRC1_ALPHA" },
			{ GL_ONE_MINUS_SRC1_ALPHA, "1 - SRC1_ALPHA" }
		};

		std::string newName;
		switch (factor)
		{
		case GL_ONE_MINUS_SRC1_ALPHA:
			factor = GL_ZERO;
			newName = enumToName[factor];
			break;
		default:
			auto it = enumToName.find(factor);
			if (it != enumToName.end())
			{
				auto &nextPair = *(++it);
				factor = nextPair.first;
				newName = nextPair.second;
			}
			else
			{
				newName = enumToName[factor] + " (Unable to modify)";
			}

			break;
		}

		glBlendFunc(GL_SRC_ALPHA, factor);

		std::string line = "Set GL blend func " + name + " to " + newName;
		EventManager::invokeEvent("Console.AddColouredLine", line, Tank::Colour::INFO);
	}
}