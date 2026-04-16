#include "imgui.h"
#include <nodes/node.hpp>
#include <nodes/scene.hpp>
#include <ui/file_dialog.hpp>
#include <ui/hierarchy.hpp>
#include <ui/inspector/inspector.hpp>
#include <ui/scene_view.hpp>
#include <ui/console.hpp>
#include <ui/profiler.hpp>
#include <project/export.hpp>
#include <project/project.hpp>
#include <reflection/node_factory.hpp>
#include <scene_serialisation.hpp>
#include <log.hpp>
#include <editor.hpp>
#include <editor_root.hpp>
#include "project_menu_bar.hpp"


namespace Tank::Editor
{
	void ProjectMenuBar::drawUI()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				drawFile();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				drawView();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}


	void ProjectMenuBar::drawFile()
	{
		if (ImGui::MenuItem("New Scene"))
		{
			fs::path scenePath = FileDialog::saveAs();
			if (scenePath == "") return;

			// Copy demo scene to the selected path
			TE_INFO(std::format("New scene > {}", scenePath.string()));
			fs::copy(
				fs::path{ "DemoProject" } / "scene.json",
				scenePath,
				fs::copy_options::overwrite_existing
			);

			// Load the scene
			auto scene = std::unique_ptr<Scene>(Serialisation::loadScene(scenePath, m_editor.getFactory()));
			EditorRoot::setScene(std::move(scene));
		}

		if (ImGui::MenuItem("Open Scene"))
		{
			fs::path scenePath = FileDialog::open(FileDialog::Target::File);
			if (scenePath == "") return;

			TE_INFO(std::format("Open scene > {}", scenePath.string()));

			// Load the scene
			auto scene = std::unique_ptr<Scene>(Serialisation::loadScene(scenePath, m_editor.getFactory()));
			EditorRoot::setScene(std::move(scene));
		}

		if (ImGui::MenuItem("Save Scene"))
		{
			fs::path scenePath = FileDialog::saveAs();
			if (scenePath == "") return;

			TE_INFO(std::format("Save scene > {}", scenePath.string()));

			// Save the scene
			Serialisation::saveScene(Scene::getActiveScene(), scenePath);
		}

		if (ImGui::MenuItem("Export Project"))
		{
			fs::path exportPath = FileDialog::open(FileDialog::Target::Directory);
			if (exportPath == "") return;

			TE_INFO(std::format("Export > {}", exportPath.string()));

			// Force save of current scene
			Tank::Serialisation::saveScene(Scene::getActiveScene(), m_editor.getProject().getSceneRes().resolvePath());

			// Export
			Export::project(
				m_editor.getProject(),
				exportPath
			);
		}
	}


	void ProjectMenuBar::drawView()
	{
		Node& root = EditorRoot::getRoot();

		if (spawnerMenuItem("Hierarchy"))
		{
			root.addChild(std::unique_ptr<_Hierarchy>(new _Hierarchy()));
		}

		if (spawnerMenuItem("Inspector"))
		{
			root.addChild(std::unique_ptr<_Inspector>(new _Inspector()));
		}

		if (spawnerMenuItem("SceneView"))
		{
			//root.addChild(std::unique_ptr<_SceneView>(new _SceneView()));
		}

		if (spawnerMenuItem("Console"))
		{
			root.addChild(std::unique_ptr<_Console>(new _Console()));
		}

		if (spawnerMenuItem("Profiler"))
		{
			root.addChild(std::unique_ptr<_Profiler>(new _Profiler()));
		}
	}


	bool ProjectMenuBar::spawnerMenuItem(const std::string& nodeName)
	{
		return ImGui::MenuItem(nodeName.c_str(), 0, false, !EditorRoot::getRoot().getChild(nodeName));
	}
}