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
#include <glad/glad.h>


const std::string g_name = "Load Model File";


namespace Tank::Editor
{
	template <>
	void NodeInspector_<Model>::draw()
	{
		ImGui::TextColored(Colour::TITLE, "Model File");
		std::string modelPath = Res::encode(m_node->getModelPath());
		Widget::textInput(
			"##Inspector_Model_File",
			modelPath,
			[this, &modelPath](const std::string &modified)
			{
				if (modified == modelPath) return;
				m_node->setModelPath(Resource::decode(modified));
				m_node->process();
			},
			modelPath
		);

		ImGui::SameLine();
		if (ImGui::SmallButton("..."))
		{
			if (!m_inspector->getSibling(g_name))
			{
				fs::path path = FileDialog::open(FileDialog::Target::Directory);
				m_node->setModelPath(path);
				m_node->process();
			}
		}

		std::vector<unsigned> options = { GL_BACK, GL_FRONT, GL_FRONT_AND_BACK };
		std::vector<std::string> optionNames = { "BACK", "FRONT", "FRONT_AND_BACK" };

		ImGui::Text("Cull Face");
		if (ImGui::BeginTable("Select Cull Face", options.size()))
		{
			for (int i = 0; i < options.size(); i++)
			{
				ImGui::TableNextColumn();
				if (ImGui::RadioButton(optionNames[i].c_str(), m_node->getCullFace() == options[i]))
				{
					m_node->setCullFace(options[i]);
				}
			}
			ImGui::EndTable();
		}
	}
}