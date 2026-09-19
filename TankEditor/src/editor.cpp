#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <nfd.h>

#include <Engine.h>
#include <nodes/Node.h>
#include <nodes/Model.h>
#include <nodes/Scene.h>
#include <nodes/Camera.h>
#include <nodes/Sprite.h>
#include <nodes/Audio.h>
#include <nodes/CubeMap.h>
#include <nodes/Light.h>
#include <nodes/physics/PhysicsBody.h>
#include <nodes/ui/UiNode.h>
#include <events/EventManager.h>
#include <scripting/Script.h>
#include <project/Project.h>
#include <project/Export.h>
#include <reflection/NodeFactory.h>
#include "Editor.h"
#include "Shader.h"
#include "Log.h"
#include "SceneSerialisation.h"
#include "KeyInput.h"
#include "ui/Console.h"
#include "ui/SceneView.h"
#include "ui/Hierarchy.h"
#include "ui/inspector/Inspector.h"
#include "ui/Profiler.h"
#include "ui/menu/ProjectsMenu.h"
#include "ui/MenuBar/GlobalMenuBar.h"
#include "ui/MenuBar/ProjectMenuBar.h"


namespace Tank::Editor
{
	std::vector<std::string> EditorApp::s_windowNames = {
		"Hierarchy",
		"Inspector",
		"SceneView",
		"Console"
	};


	EditorApp::EditorApp() : Application(
		true,
		{
			ImGuiConfigFlags_DockingEnable,
			ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoBringToFrontOnFocus
		}
	)
	{
		TE_INFO("Editor > Constructor");
		TE_INFO(std::format("cwd: {}", fs::current_path().string()));
		TE_INFO(std::format("core-assets: {}", Res::getCoreAssetsPath().string()));

		// Create Editor KeyInput
		std::vector<int> registeredKeys = {
			// Function keys
			GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4, GLFW_KEY_F5, GLFW_KEY_F6,
			// Cam Movement keys
			GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_Q, GLFW_KEY_E,
			// Cam Rotation keys
			GLFW_KEY_I, GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L, GLFW_KEY_U, GLFW_KEY_O,
		};
		m_editorInput = std::make_unique<KeyInput>(registeredKeys);

		// Add editor-specific events
		EventManager::addEvent("Hierarchy.NodeSelected", new Event<Node*>());
		EventManager::addEvent("Hierarchy.NodeDeleted", new Event<Node*>());
		EventManager::addEvent("Console.AddColouredLine", new Event<std::string, ImColor>());

		// Set the ImGui context, over DLL boundary
		ImGui::SetCurrentContext(getContext());

		m_initUI = std::make_unique<Node>("EditorInit");
		m_projectUI = nullptr;
		m_sceneRoot = std::make_unique<Node>("EditorScene");
		m_project = nullptr;
	
		// Load Project (via New/Open Project)
		auto projectsMenu = std::unique_ptr<ProjectsMenu>(
			new ProjectsMenu(
				[this](const fs::path &path)
				{
					// Try to load the project (exit if this fails)
					m_project = Project::loadFromDir(path);
					if (m_project == nullptr) return;

					// Update cwd to project path
					fs::current_path(path);

					// Load the scene
					Res scenePath = m_project->getSceneRes();
					if (_Scene *rawScene = Tank::Serialisation::loadScene(scenePath.resolvePathStr(), *m_factory))
					{
						m_projectUI = std::make_unique<Node>("EditorProject");
						m_projectUI->addChild(std::unique_ptr<ProjectMenuBar_>(new ProjectMenuBar_(*this)));
						m_projectUI->addChild(std::unique_ptr<SceneView_>(new SceneView_("SceneView", getWindowSize(), getWindowSize(), m_editorInput.get())));
						m_projectUI->addChild(std::unique_ptr<Console_>(new Console_("Console")));
						m_projectUI->addChild(std::unique_ptr<Hierarchy_>(new Hierarchy_("Hierarchy")));
						m_projectUI->addChild(std::unique_ptr<Inspector_>(new Inspector_("Inspector")));
						m_projectUI->preupdate();

						setScene(std::unique_ptr<_Scene>(rawScene));
						m_sceneRoot->preupdate();
					}
				}
			)
		);

		m_initUI->addChild(std::unique_ptr<GlobalMenuBar_>(new GlobalMenuBar_(*this)));
		m_initUI->addChild(std::move(projectsMenu));
	}


	EditorApp::~EditorApp()
	{
	}

	
	void EditorApp::setScene(std::unique_ptr<_Scene> scene)
	{
		assert(m_sceneRoot != nullptr);
	
		std::vector<_Scene *> existingScenes = m_sceneRoot->getChildrenOfType<_Scene>();
		assert(existingScenes.size() <= 1);
		if (existingScenes.size() > 0) existingScenes[0]->destroy();

		m_sceneRoot->addChild(std::move(scene));
	}


	void EditorApp::step()
	{
		if (m_editorInput)
		{
			handleKeyInput();
			// Decay input states (comes after handleKeyInput)
			m_editorInput->update();
		}

		// Run update step on root
		if (m_sceneRoot)
		{
			m_sceneRoot->update();
		}

		// Run update step on project UI
		if (m_projectUI)
		{
			m_projectUI->update();
		}
	}


	// Currently, the Editor handles drawUI
	void EditorApp::uiStep()
	{
		// Run update step on initUI
		m_initUI->update();
		// Run ImGui update step on initUI and its children
		auto initUINodes = m_initUI->getChildrenOfType<UINode>();
		for (const auto uiNode : initUINodes)
		{
			uiNode->drawUI();
		}

		if (m_projectUI)
		{
			auto uiNodes = m_projectUI->getChildrenOfType<UINode>();
			for (const auto uiNode : uiNodes)
			{
				uiNode->drawUI();
			}
		}
	}


	void EditorApp::handleKeyInput()
	{
		if (m_projectUI)
		{
			((SceneView_*)m_projectUI->getChild("SceneView"))->handleKeyInput();
		}
	}
}


namespace Tank
{
	std::unique_ptr<Tank::Application> createApplication()
	{
		return std::make_unique<Editor::EditorApp>();
	}
}
