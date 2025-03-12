#include <format>
#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "log.hpp"
#include "framebuffer.hpp"
#include "key_input.hpp"
#include "colours.hpp"
#include "static/time.hpp"
#include "nodes/scene.hpp"
#include "nodes/ui/scene_view.hpp"
#include "nodes/ui/console.hpp"
#include "nodes/ui/inspector.hpp"


namespace Tank::Editor
{
	_SceneView::_SceneView(const std::string &name, glm::ivec2 sceneViewportSize, glm::ivec2 fbViewportSize, KeyInput *keyInput)
		: _Window(name)
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
	}


	void _SceneView::drawUI()
	{
		int fbW = m_fb->getW(), fbH = m_fb->getH();

		// Just sets default panel-window size.
		ImGui::SetNextWindowSize(ImVec2(fbW + 10.0f, fbH + 10.0f), ImGuiCond_FirstUseEver);
		_Window::drawUI();
	}


	void _SceneView::drawPanel()
	{
		int fbW = m_fb->getW(), fbH = m_fb->getH();

		ImGui::BeginChild("SceneRender");
		m_isFocussed = ImGui::IsWindowFocused();

		ImVec2 wsize = ImGui::GetWindowSize();
		int fbWNew = (int)wsize.x - 10;
		int fbHNew = (int)wsize.y - 10;

		if (fbWNew != fbW || fbHNew != fbH)
			rescale(fbWNew, fbHNew);

		ImVec2 fbsize = ImVec2((float)fbW, (float)fbH);
		ImTextureID imTex = (ImTextureID)(intptr_t)m_fb->getTexColBuf();

		// Play mode button (once clicked, hide the button and replace it with Stop)
		if (m_isInPlayMode || ImGui::Button("Play"))
		{
			m_isInPlayMode = true;

			// Hide all UI other than this SceneView.
			for (auto &node : *getParent())
			{
				if (node.get() == this) continue;
				node->setEnabled(false);
			}
			// Start all scripting.
			Scene *activeScene = Scene::getActiveScene();
			activeScene->getActiveCamera()->setFreeLook(false);
			activeScene->startup();
			

			if (ImGui::Button("Stop"))
			{
				m_isInPlayMode = false;
				Scene *activeScene = Scene::getActiveScene();
				activeScene->getActiveCamera()->setFreeLook(true);
				activeScene->shutdown();

				// Show all UI other than this SceneView.
				for (auto &node : *getParent())
				{
					if (node.get() == this) continue;
					node->setEnabled(true);
				}
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
		ImGui::EndChild();
	}


	void _SceneView::update()
	{
		int fbW = m_fb->getW(), fbH = m_fb->getH();

		// Set viewport to fbo, render into fbo, set viewport to std.
		glViewport(0, 0, fbW, fbH);
		m_fb->update();
		glViewport(0, 0, m_sceneW, m_sceneH);

		UI::update();
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

		dynamic_cast<_Console*>(getSibling("Console"))->addColouredTextLine(Tank::Colour::INFO, line);
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

		dynamic_cast<_Console*>(getSibling("Console"))->addColouredTextLine(Tank::Colour::INFO, line);
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

		dynamic_cast<_Console*>(getSibling("Console"))->addColouredTextLine(Tank::Colour::INFO, line);
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

		dynamic_cast<_Console*>(getSibling("Console"))->addColouredTextLine(
			Tank::Colour::INFO,
			"Set GL depth func comparison to " + newMode
		);
	}
}