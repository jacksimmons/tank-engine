#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <nfd.h>

#include <engine.hpp>
#include <nodes/node.hpp>
#include <nodes/model.hpp>
#include <nodes/scene.hpp>
#include <nodes/camera.hpp>
#include <nodes/sprite.hpp>
#include <nodes/audio.hpp>
#include <nodes/cube_map.hpp>
#include <nodes/light.hpp>
#include <nodes/physics/physics_body.hpp>
#include <nodes/ui/ui_node.hpp>
#include <events/event_manager.hpp>
#include <scripting/script.hpp>
#include <project/project.hpp>
#include <project/export.hpp>
#include <reflection/node_factory.hpp>
#include "editor.hpp"
#include "shader.hpp"
#include "log.hpp"
#include "scene_serialisation.hpp"
#include "key_input.hpp"
#include "ui/console.hpp"
#include "ui/scene_view.hpp"
#include "ui/hierarchy.hpp"
#include "ui/inspector/inspector.hpp"
#include "ui/profiler.hpp"
#include "ui/menu/projects_menu.hpp"
#include "ui/menu_bar/global_menu_bar.hpp"
#include "ui/menu_bar/project_menu_bar.hpp"


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
		TE_INFO(std::format("CWD: {}", fs::current_path().string()));
		TE_INFO(std::format("CoreAssets: {}", Res::getCoreAssetsPath().string()));

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

		m_initUI = std::make_unique<Node>("Init");
		m_project = nullptr;
	
		// Load Project (via New/Open Project)
		auto projectsMenu = std::unique_ptr<ProjectsMenu>(
			new ProjectsMenu(
				[this](const fs::path &path)
				{
					// Load the project
					m_project = Project::loadFromDir(path);

					// Update cwd to project path
					fs::current_path(path);

					// Load the scene
					Res scenePath = m_project->getSceneRes();
					if (Scene *rawScene = Tank::Serialisation::loadScene(scenePath.resolvePathStr(), *m_factory))
					{
						m_projRoot = std::make_unique<Node>("Editor");
						setScene(std::unique_ptr<Scene>(rawScene));

						m_projRoot->addChild(std::unique_ptr<_ProjectMenuBar>(new _ProjectMenuBar(*this)));
						m_projRoot->addChild(std::unique_ptr<_SceneView>(new _SceneView("SceneView", getWindowSize(), getWindowSize(), m_editorInput.get())));
						m_projRoot->addChild(std::unique_ptr<_Console>(new _Console("Console")));
						m_projRoot->addChild(std::unique_ptr<_Hierarchy>(new _Hierarchy("Hierarchy")));
						m_projRoot->addChild(std::unique_ptr<_Inspector>(new _Inspector("Inspector")));
						m_projRoot->preupdate();
					}
				}
			)
		);

		m_initUI->addChild(std::unique_ptr<_GlobalMenuBar>(new _GlobalMenuBar(*this)));
		m_initUI->addChild(std::move(projectsMenu));
	}


	EditorApp::~EditorApp()
	{
	}

	
	void EditorApp::setScene(std::unique_ptr<Scene> scene)
	{
		assert(m_projRoot != nullptr);
	
		std::vector<Scene *> existingScenes = m_projRoot->getChildrenOfType<Scene>();
		assert(existingScenes.size() <= 1);
		if (existingScenes.size() > 0) existingScenes[0]->destroy();

		m_projRoot->addChild(std::move(scene));
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
		if (m_projRoot)
		{
			m_projRoot->update();
		}
	}


	// Currently, the Editor handles drawUI
	void EditorApp::uiStep()
	{
		// Run ImGui update step on root
		if (m_projRoot)
		{
			auto uiNodes = m_projRoot->getChildrenOfType<UINode>();
			for (const auto uiNode : uiNodes)
			{
				uiNode->drawUI();
			}
		}

		// Run update step on initUI
		m_initUI->update();
		// Run ImGui update step on initUI and its children
		auto initUINodes = m_initUI->getChildrenOfType<UINode>();
		for (const auto uiNode : initUINodes)
		{
			uiNode->drawUI();
		}
	}


	void EditorApp::handleKeyInput()
	{
		if (m_projRoot)
		{
			((_SceneView*)m_projRoot->getChild("SceneView"))->handleKeyInput();
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
