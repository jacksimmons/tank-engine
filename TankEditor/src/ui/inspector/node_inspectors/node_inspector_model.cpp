#include <imgui.h>
#include <colours.hpp>
#include <widget.hpp>
#include <nodes/model.hpp>
#include "node_inspector.hpp"
#include "../inspector.hpp"
#include "../../file_dialog.hpp"


namespace Tank::Editor
{
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
			std::string name = "Load Model File";
			if (!m_inspector->getSibling(name))
			{
				auto fileDialog = std::unique_ptr<_FileDialog>(
					new _FileDialog(
						name,
						fs::current_path(),
						fs::path(modelPath).parent_path(),
						_FileDialogTarget::File,
						[this](const fs::path &path)
						{
							// Only update the model if user has selected a valid file
							if (!path.has_filename()) return;
							m_node->setModelPath(path);
						}
					));

				m_inspector->getParent()->addChild(std::move(fileDialog));
			}
		}
	}
}