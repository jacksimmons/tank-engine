#include <glad/glad.h>
#include "imgui.h"
#include <fs/file.hpp>
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
#include <colours.hpp>
#include <log.hpp>
#include <editor.hpp>
#include "project_menu_bar.hpp"


namespace Tank::Editor
{
	void ProjectMenuBar_::drawMainMenuBar()
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
	}


	void ProjectMenuBar_::drawFile()
	{
		bool resetInspector = false;

		if (ImGui::MenuItem("New Scene"))
		{
			fs::path scenePath = FileDialog::saveAs();
			if (scenePath == "") return;

			// Copy demo scene to the selected path
			TE_INFO(std::format("New scene > {}", scenePath.string()));
			File::tryCopy(
				fs::absolute(fs::path(TANK_WKS)) / "demo-project" / "assets" / "scene.json",
				scenePath,
				fs::copy_options::overwrite_existing
			);

			// Load the scene
			auto scene = std::unique_ptr<Scene>(Serialisation::loadScene(scenePath, m_editor.getFactory()));
			m_editor.setScene(std::move(scene));

			resetInspector = true;
		}

		if (ImGui::MenuItem("Open Scene"))
		{
			fs::path scenePath = FileDialog::open(FileDialog::Target::File);
			if (scenePath == "") return;

			TE_INFO(std::format("Open scene > {}", scenePath.string()));

			// Load the scene
			auto scene = std::unique_ptr<Scene>(Serialisation::loadScene(scenePath, m_editor.getFactory()));
			m_editor.setScene(std::move(scene));

			resetInspector = true;
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

		// If we did anything that would corrupt the inspector's inspected node, reset the inspector.
		if (resetInspector)
		{
			Inspector_ *inspector = dynamic_cast<Inspector_ *>(m_editor.getProjRoot().getChild("Inspector"));
			assert(inspector != nullptr);

			inspector->m_inspectedNode = nullptr;
		}
	}


	void ProjectMenuBar_::drawView()
	{
		Node& root = m_editor.getProjRoot();

		if (spawnerMenuItem("Hierarchy"))
		{
			root.addChild(std::unique_ptr<Hierarchy_>(new Hierarchy_()));
		}

		if (spawnerMenuItem("Inspector"))
		{
			root.addChild(std::unique_ptr<Inspector_>(new Inspector_()));
		}

		if (spawnerMenuItem("SceneView"))
		{
			//root.addChild(std::unique_ptr<SceneView_>(new SceneView_()));
		}

		if (spawnerMenuItem("Console"))
		{
			root.addChild(std::unique_ptr<Console_>(new Console_()));
		}

		if (spawnerMenuItem("Profiler"))
		{
			root.addChild(std::unique_ptr<Profiler_>(new Profiler_()));
		}
	}


	bool ProjectMenuBar_::spawnerMenuItem(const std::string& nodeName)
	{
		return ImGui::MenuItem(nodeName.c_str(), 0, false, !m_editor.getProjRoot().getChild(nodeName));
	}
}