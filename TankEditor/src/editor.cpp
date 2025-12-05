#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include "editor.hpp"
#include "shader.hpp"
#include "log.hpp"
#include "scene_serialisation.hpp"
#include <events/event_manager.hpp>
#include "nodes/node.hpp"
#include "nodes/camera.hpp"
#include "nodes/scene.hpp"
#include "nodes/model.hpp"
#include "nodes/light.hpp"
#include "nodes/cube_map.hpp"
#include "nodes/sprite.hpp"
#include "ui/console.hpp"
#include "ui/scene_view.hpp"
#include "ui/hierarchy.hpp"
#include "ui/inspector/inspector.hpp"
#include "ui/file_dialog.hpp"
#include "ui/main_menu_bar.hpp"
#include "ui/profiler.hpp"
#include "nodes/physics/physics_body.hpp"


namespace Tank::Editor
{
	std::vector<std::string> EditorApp::s_windowNames = {
		"Hierarchy",
		"Inspector",
		"SceneView",
		"Console"
	};


	EditorApp::EditorApp() : Application(
		{
			GLFW_KEY_F1,
			GLFW_KEY_F2,
			GLFW_KEY_F3,
			GLFW_KEY_F4,
			GLFW_KEY_F5,
			GLFW_KEY_F6,

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
			GLFW_KEY_O,
		},
		{
			ImGuiConfigFlags_DockingEnable,
			ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoBringToFrontOnFocus
		}
	)
	{
		// Add editor-specific events
		EventManager::addEvent("Hierarchy.NodeSelected", new Event<Node*>());
		EventManager::addEvent("Hierarchy.NodeDeleted", new Event<Node*>());
		EventManager::addEvent("FileDialog.ItemSelected", new Event<_FileDialog*, fs::path>());

		// Register file dialog event handlers
		EventManager::getEvent<_FileDialog*, fs::path>("FileDialog.ItemSelected")
		->registerHandler(
			[this](_FileDialog *dialog, const std::filesystem::path &path)
			{
				const std::string &name = dialog->getName();

				if (name == "Open Project")
				{
					std::unique_ptr<::Tank::Scene> scene;

					// Load scene if it was valid, and close the popup either way
					if (Scene *rawScene = ::Tank::Serialisation::loadScene(path.string(), m_factory.get()))
					{
						scene = std::unique_ptr<::Tank::Scene>(rawScene);
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
		m_initUI->addChild(
			std::unique_ptr<_MainMenuBar>(new _MainMenuBar("MainMenuBar",
			{
				{
					"File",
					[]() { return true; },
					{
						{
							"New Project",
							[]() { return true; },
							[this]()
							{
								loadDemoScene();
								postSceneSetup();
							}
						},
						{
							"Open Project",
							[this]() { return !m_initUI->getChild("Open Project"); },
							[this]()
							{
								std::unique_ptr<_FileDialog> fileDialog = std::unique_ptr<_FileDialog>(
									new _FileDialog("Open Project", "", fs::current_path(), _FileDialogTarget::File)
								);
								m_initUI->addChild(std::move(fileDialog));
							}
						},
						{
							"Save Project",
							[this]() { return Scene::getActiveScene() && !m_initUI->getChild("Save Project"); },
							[this]()
							{
								std::unique_ptr<_FileDialog> fileDialog = std::unique_ptr<_FileDialog>(
									new _FileDialog("Save Project", fs::current_path(), fs::current_path(), _FileDialogTarget::File)
								);
								m_initUI->addChild(std::move(fileDialog));
							}
						}
					},
				},

				{
					"Window",
					[]() { return Scene::getActiveScene(); },
					{
						{
							"Hierarchy",
							[this]() { return !m_system->getChild("Hierarchy"); },
							[this]() { m_system->addChild(std::unique_ptr<_Hierarchy>(new _Hierarchy())); }
						},
						{
							"Inspector",
							[this]() { return !m_system->getChild("Inspector"); },
							[this]() { m_system->addChild(std::unique_ptr<_Inspector>(new _Inspector())); }
						},
						{
							"SceneView",
							[this]() { return !m_system->getChild("SceneView"); },
							//[this]() { m_system->addChild(std::make_unique<_SceneView>(); }
						},
						{
							"Console",
							[this]() { return !m_system->getChild("Console"); },
							[this]() { m_system->addChild(std::unique_ptr<_Console>(new _Console())); }
						},
						{
							"Profiler",
							[this]() { return !m_system->getChild("Profiler"); },
							[this]() { m_system->addChild(std::unique_ptr<_Profiler>(new _Profiler())); }
						}
					}
				}
			}))
		);
	}


	EditorApp::~EditorApp()
	{
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

				auto object = std::unique_ptr<::Tank::Model>(new Model("Doom", fs::current_path() / "models/doom/doom_E1M1.obj", &sources));
				object->getTransform()->setLocalTranslation({ 0, 0, 0 });
				scene->addChild(std::move(object));

				auto backpackPhysics = std::unique_ptr<::Tank::PhysicsBody>(new PhysicsBody("BackpackBody", 1e15f));
				auto backpack = std::unique_ptr<::Tank::Model>(new Model("Backpack", fs::current_path() / "models/backpack/backpack.obj", &sources));
				backpack->getTransform()->setLocalScale({ 100, 100, 100 });
				backpackPhysics->getTransform()->setLocalTranslation({ 0, 0, 200 });
				backpackPhysics->addChild(std::move(backpack));
				scene->addChild(std::move(backpackPhysics));

				auto spritePhysics = std::unique_ptr<::Tank::PhysicsBody>(new PhysicsBody("SpriteBody", 1e15f));
				auto sprite = std::unique_ptr<::Tank::Sprite>(new Sprite("Sprite", fs::current_path() / "textures/awesomeface.png", &sources));
				spritePhysics->addChild(std::move(sprite));
				scene->addChild(std::move(spritePhysics));
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
		m_system->addChild(std::unique_ptr<_SceneView>(new _SceneView("SceneView", getWindowSize(), getWindowSize(), m_keyInput.get())));
		m_system->addChild(std::unique_ptr<_Console>(new _Console("Console")));
		m_system->addChild(std::unique_ptr<_Hierarchy>(new _Hierarchy("Hierarchy")));
		m_system->forEachDescendant([](::Tank::Node *node) { TE_CORE_INFO(node->getName()); });
		m_system->addChild(std::unique_ptr<_Inspector>(new _Inspector("Inspector")));
		m_system->preupdate();
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
			TE_CORE_WARN("handleKeyInput: System not initialised. Returning");
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
