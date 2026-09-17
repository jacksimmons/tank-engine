#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <nfd.h>

#include <Engine.hpp>
#include <nodes/Node.hpp>
#include <nodes/Model.hpp>
#include <nodes/Scene.hpp>
#include <nodes/Camera.hpp>
#include <nodes/Sprite.hpp>
#include <nodes/Audio.hpp>
#include <nodes/CubeMap.hpp>
#include <nodes/Light.hpp>
#include <nodes/physics/PhysicsBody.hpp>
#include <nodes/ui/UiNode.hpp>
#include <events/EventManager.hpp>
#include <scripting/Script.hpp>
#include <project/Project.hpp>
#include <project/Export.hpp>
#include <reflection/NodeFactory.hpp>
#include "Editor.hpp"
#include "Shader.hpp"
#include "Log.hpp"
#include "SceneSerialisation.hpp"
#include "KeyInput.hpp"
#include "ui/Console.hpp"
#include "ui/SceneView.hpp"
#include "ui/Hierarchy.hpp"
#include "ui/inspector/Inspector.hpp"
#include "ui/Profiler.hpp"
#include "ui/menu/ProjectsMenu.hpp"
#include "ui/MenuBar/GlobalMenuBar.hpp"
#include "ui/MenuBar/ProjectMenuBar.hpp"


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
					if (Scene *rawScene = Tank::Serialisation::loadScene(scenePath.resolvePathStr(), *m_factory))
					{
						m_projectUI = std::make_unique<Node>("EditorProject");
						m_projectUI->addChild(std::unique_ptr<ProjectMenuBar_>(new ProjectMenuBar_(*this)));
						m_projectUI->addChild(std::unique_ptr<SceneView_>(new SceneView_("SceneView", getWindowSize(), getWindowSize(), m_editorInput.get())));
						m_projectUI->addChild(std::unique_ptr<Console_>(new Console_("Console")));
						m_projectUI->addChild(std::unique_ptr<Hierarchy_>(new Hierarchy_("Hierarchy")));
						m_projectUI->addChild(std::unique_ptr<Inspector_>(new Inspector_("Inspector")));
						m_projectUI->preupdate();

						setScene(std::unique_ptr<Scene>(rawScene));
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

	
	void EditorApp::setScene(std::unique_ptr<Scene> scene)
	{
		assert(m_sceneRoot != nullptr);
	
		std::vector<Scene *> existingScenes = m_sceneRoot->getChildrenOfType<Scene>();
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
