#include <imgui.h>
#include <colours.hpp>
#include <nodes/scene.hpp>
#include <nodes/camera.hpp>
#include "node_inspector.hpp"


namespace Tank::Editor
{
	/// <summary>
	/// Draws a section specifically for the root node (scene).
	/// </summary>
	template <>
	void _NodeInspector<Scene>::draw()
	{
		ImGui::TextColored(Colour::TITLE, "Active Camera");

		// Determine active camera text name and colour.
		std::string cameraName;
		ImColor cameraNameCol;
		Camera *activeCamera = m_node->getActiveCamera();
		if (activeCamera)
		{
			cameraName = activeCamera->getName();
			cameraNameCol = Colour::NORMAL;
		}
		else
		{
			cameraName = "NULL";
			cameraNameCol = Colour::DISABLED;
		}

		ImGui::SetNextItemAllowOverlap();
		ImGui::TextColored(cameraNameCol, cameraName.c_str());
		ImGui::SameLine();

		// Display a button to change the active camera.
		// Clicking on it loads a list of all Camera nodes descending from this scene.
		if (ImGui::SmallButton("Set##INSPECTOR_SCENE_SET_CAM"))
		{
			ImGui::OpenPopup("##INSPECTOR_SCENE_SET_CAM_LIST");
		}

		if (ImGui::BeginPopup("##INSPECTOR_SCENE_SET_CAM_LIST"))
		{
			m_node->forEachDescendant(
				[this](Node *node)
				{
					if (Camera *cam = dynamic_cast<Camera *>(node))
					{
						if (ImGui::Button((cam->getPath() + "##INSPECTOR_SCENE_SET_CAM_LIST_BTN").c_str()))
						{
							m_node->setActiveCamera(cam);
						}
					}
				}
			);

			ImGui::EndPopup();
		}
	}
}