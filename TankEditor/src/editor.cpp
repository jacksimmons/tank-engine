#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <nodes/node.hpp>
#include <nodes/model.hpp>
#include <nodes/scene.hpp>
#include <nodes/camera.hpp>
#include <nodes/sprite.hpp>
#include <nodes/audio.hpp>
#include <nodes/cube_map.hpp>
#include <nodes/light.hpp>
#include <nodes/physics/physics_body.hpp>
#include <events/event_manager.hpp>
#include <scripting/script.hpp>
#include <export/bundle_builder.hpp>
#include "editor.hpp"
#include "shader.hpp"
#include "log.hpp"
#include "scene_serialisation.hpp"
#include "key_input.hpp"
#include "ui/console.hpp"
#include "ui/scene_view.hpp"
#include "ui/hierarchy.hpp"
#include "ui/inspector/inspector.hpp"
#include "ui/file_dialog.hpp"
#include "ui/main_menu_bar.hpp"
#include "ui/profiler.hpp"


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
		EventManager::addEvent("FileDialog.ItemSelected", new Event<_FileDialog*, fs::path>());
		EventManager::addEvent("Console.AddColouredLine", new Event<std::string, ImColor>());

		// Register file dialog event handlers
		EventManager::getEvent<_FileDialog*, fs::path>("FileDialog.ItemSelected")
		->registerHandler(
			[this](_FileDialog *dialog, const std::filesystem::path &path)
			{
				const std::string &name = dialog->getName();

				if (name == "Open Project")
				{
					// Load scene if it was valid, and close the popup either way
					if (Scene *rawScene = ::Tank::Serialisation::loadScene(path.string(), m_factory.get()))
					{
						std::unique_ptr<::Tank::Scene> scene = std::unique_ptr<::Tank::Scene>(rawScene);
						loadScene(std::move(scene));
						postSceneSetup();
					}
				}
				else if (name == "Save Project")
				{
					Serialisation::saveScene(Scene::getActiveScene(), path);
				}
			}
		);

		// Set the ImGui context, over DLL boundary
		ImGui::SetCurrentContext(getContext());

		// Initialise UI
		m_initUI = std::make_unique<Node>("Init");

		// Initialise Tabs
		std::vector<Tab> tabs = { getFileTab(), getWindowTab() };
		m_initUI->addChild(std::unique_ptr<_MainMenuBar>(new _MainMenuBar("MainMenuBar", tabs)));
	}


	EditorApp::~EditorApp()
	{
	}


	Tab EditorApp::getFileTab()
	{
		TabItem newProj =
		{
			"New Project",
			[](auto &) { return true; },
			[this]()
			{
				loadDemoScene();
				postSceneSetup();
			}
		};

		TabItem openProj =
		{
			"Open Project...",
			[this](auto &name) { return !m_initUI->getChild(name); },
			[this]()
			{
				std::unique_ptr<_FileDialog> fileDialog = std::unique_ptr<_FileDialog>(
					new _FileDialog("Open Project", "", fs::current_path(), _FileDialogTarget::File)
				);
				m_initUI->addChild(std::move(fileDialog));
			}
		};

		TabItem saveProj = {
			"Save Project As...",
			[this](auto &name) { return Scene::getActiveScene() && !m_initUI->getChild(name); },
			[this]()
			{
				std::unique_ptr<_FileDialog> fileDialog = std::unique_ptr<_FileDialog>(
					new _FileDialog("Save Project", fs::current_path(), fs::current_path(), _FileDialogTarget::File)
				);
				m_initUI->addChild(std::move(fileDialog));
			}
		};

		TabItem exportProj = {
			"Export Project...",
			[this](auto &name) { return Scene::getActiveScene() && !m_initUI->getChild(name); },
			[this]()
			{
				std::unique_ptr<_FileDialog> fd = std::unique_ptr<_FileDialog>(
					new _FileDialog(
						"Export Project",
						fs::current_path().root_directory(),
						fs::current_path(),
						_FileDialogTarget::Directory,
						[this](const fs::path &path)
						{
							Export::BundleBuilder::build(Scene::getActiveScene(), path);
						}
					)
				);
				m_initUI->addChild(std::move(fd));
			}
		};

		Tab file =
		{
			"File",
			[](auto &) { return true; },
			{ newProj, openProj, saveProj, exportProj }
		};
		return file;
	}


	Tab EditorApp::getWindowTab()
	{
		TabItem hierarchy =
		{
			"Hierarchy",
			[this](auto &name) { return !m_system->getChild(name); },
			[this]() { m_system->addChild(std::unique_ptr<_Hierarchy>(new _Hierarchy())); }
		};

		TabItem inspector =
		{
			"Inspector",
			[this](auto &name) { return !m_system->getChild(name); },
			[this]() { m_system->addChild(std::unique_ptr<_Inspector>(new _Inspector())); }
		};

		TabItem sceneView =
		{
			"SceneView",
			[this](auto &name) { return !m_system->getChild(name); },
			//[this]() { m_system->addChild(std::make_unique<_SceneView>(); }
		};

		TabItem console =
		{
			"Console",
			[this](auto &name) { return !m_system->getChild(name); },
			[this]() { m_system->addChild(std::unique_ptr<_Console>(new _Console())); }
		};

		TabItem profiler =
		{
			"Profiler",
			[this](auto &name) { return !m_system->getChild(name); },
			[this]() { m_system->addChild(std::unique_ptr<_Profiler>(new _Profiler())); }
		};

		Tab window =
		{
			"Window",
			[](auto &) { return Scene::getActiveScene(); },
			{ hierarchy, inspector, sceneView, console, profiler }
		};
		return window;
	}


	void EditorApp::loadScene(std::unique_ptr<Scene> scene)
	{
		m_system.reset();
		m_system = std::make_unique<Node>("Editor");
		m_system->addChild(std::move(scene));
	}


	void EditorApp::loadDemoScene()
	{
		// Create nodes
		{
			auto scene = std::make_unique<::Tank::Scene>();
			{
				auto camera = std::make_unique<::Tank::Camera>();
				scene->setActiveCamera(dynamic_cast<::Tank::Camera*>(camera.get()));
				scene->addChild(std::move(camera));
			}
			{
				ShaderSources sources;
				sources.vertex.location = "skybox.vert";
				sources.fragment.location = "skybox.frag";
				scene->addChild(std::make_unique<::Tank::CubeMap>("CubeMap", &sources));
			}
			{
				ShaderSources sources;
				sources.vertex.location = "shader.vert";
				sources.fragment.location = "shader.frag";

				auto doom = std::unique_ptr<::Tank::Model>(new Model("Doom", fs::current_path() / "models/doom/doom_E1M1.obj", &sources));
				doom->getTransform()->setLocalTranslation({ 0, 0, 0 });
				scene->addChild(std::move(doom));

				auto player = std::unique_ptr<::Tank::Model>(new Model("Player", fs::current_path() / "models/player/Fat Banjo and Kazooie.obj", &sources));
				player->addScript(Script::createScript(player.get(), "player.lua").value());
				player->setCullFace(GL_FRONT);
				scene->addChild(std::move(player));

				//auto backpackPhysics = std::unique_ptr<::Tank::PhysicsBody>(new PhysicsBody("BackpackBody", 1e15f));
				//auto backpack = std::unique_ptr<::Tank::Model>(new Model("Backpack", fs::current_path() / "models/backpack/backpack.obj", &sources));
				//backpack->getTransform()->setLocalScale({ 100, 100, 100 });
				//backpackPhysics->getTransform()->setLocalTranslation({ 0, 0, 200 });
				//backpackPhysics->addChild(std::move(backpack));
				//scene->addChild(std::move(backpack));

				//auto spritePhysics = std::unique_ptr<::Tank::PhysicsBody>(new PhysicsBody("SpriteBody", 1e15f));
				//auto sprite = std::unique_ptr<::Tank::Sprite>(new Sprite("Sprite", fs::current_path() / "textures/awesomeface.png", &sources));
				//spritePhysics->addChild(std::move(sprite));
				//scene->addChild(std::move(sprite));

				auto audio = std::make_unique<::Tank::Audio>();
				scene->addChild(std::move(audio));
			}

			Scene::setActiveScene(scene.get());
			loadScene(std::move(scene));

			// Lights can only be added after scene load
			std::string name = "DirLight";
			auto light = std::make_unique<::Tank::DirLight>(name,
				glm::vec3{ 0.0f, -1.0f, 0.0f },
				glm::vec3{ 0.02f, 0.02f, 0.02f },
				glm::vec3{ 0.2f, 0.2f, 0.2f },
				glm::vec3{ 0.1f, 0.1f, 0.1f }
			);
			Scene::getActiveScene()->addChild(std::move(light));
		}
	}


	void EditorApp::postSceneSetup()
	{
		m_system->addChild(std::unique_ptr<_SceneView>(new _SceneView("SceneView", getWindowSize(), getWindowSize(), m_editorInput.get())));
		m_system->addChild(std::unique_ptr<_Console>(new _Console("Console")));
		m_system->addChild(std::unique_ptr<_Hierarchy>(new _Hierarchy("Hierarchy")));
		m_system->forEachDescendant([](::Tank::Node *node) { TE_CORE_INFO(node->getName()); });
		m_system->addChild(std::unique_ptr<_Inspector>(new _Inspector("Inspector")));
		m_system->preupdate();
	}


	void EditorApp::step()
	{
		if (m_editorInput)
		{
			handleKeyInput();
			// Decay input states (comes after handleKeyInput)
			m_editorInput->update();
		}
	}
	

	void EditorApp::uiStep()
	{
		// Draw all system UI (SceneView/Framebuffer draws the scene)
		if (m_system)
		{
			m_system->update();
		}
		m_initUI->update();
	}


	void EditorApp::handleKeyInput()
	{
		if (!m_system)
		{
			return;
		}
		((_SceneView*)m_system->getChild("SceneView"))->handleKeyInput();
	}
}


namespace Tank
{
	std::unique_ptr<Tank::Application> createApplication()
	{
		return std::make_unique<Editor::EditorApp>();
	}
}
