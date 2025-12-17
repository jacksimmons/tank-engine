#include <filesystem>
#include "../inspector.hpp"
#include "node_inspector.hpp"
#include <colours.hpp>
#include <events/event.hpp>
#include <events/event_manager.hpp>
#include <imgui.h>
#include <nodes/model.hpp>
#include "ui/file_dialog.hpp"
#include <widget.hpp>


const std::string g_name = "Load Model File";


namespace Tank::Editor
{
	template <>
	void _NodeInspector<Model>::draw()
	{
		ImGui::TextColored(Colour::TITLE, "Model File");
		std::string modelPath = m_node->getModelPath().string();
		Widget::textInput(
			"##Inspector_Model_File",
			modelPath,
			[this](const std::string &modified)
			{
				if (fs::path{ modified } == m_node->getModelPath()) return;
				m_node->setModelPath(modified);
			},
			modelPath
		);

		ImGui::SameLine();
		if (ImGui::SmallButton("..."))
		{
			if (!m_inspector->getSibling(g_name))
			{
				auto fileDialog = std::make_unique<_FileDialog>(
					g_name,
					fs::current_path(),
					fs::path(modelPath).parent_path(),
					_FileDialogTarget::File
				);

				m_inspector->addChild(std::move(fileDialog));
			}
		}
	}
}