#include <imgui.h>
#include <widget.hpp>
#include <colours.hpp>
#include <nodes/audio.hpp>
#include "node_inspector.hpp"
#include "../inspector.hpp"


const std::string g_name = "Load Audio File";

namespace Tank::Editor
{
	/// <summary>
	/// Draws inspector section that is present for all Audios.
	/// </summary>
	template <>
	void _NodeInspector<Audio>::draw()
	{
		ImGui::TextColored(Colour::TITLE, "Audio File");
		auto &audioPath = m_node->AudioPath();
		Widget::textInput(
			"##Inspector_Audio_File",
			audioPath.string(),
			[this](const std::string &modified)
			{
				if (fs::path{ modified } == m_node->AudioPath()) return;
				//m_node->AudioPath = modified;
			},
			audioPath.string()
		);

		ImGui::SameLine();
		if (ImGui::SmallButton("..."))
		{
			if (!m_inspector->getSibling(g_name))
			{
				auto fileDialog = std::make_unique<_FileDialog>(
					g_name,
					fs::current_path(),
					fs::path(audioPath).parent_path(),
					_FileDialogTarget::File
				);

				m_inspector->addChild(std::move(fileDialog));
			}
		}

		ImGui::SameLine();
		if (ImGui::SmallButton("Play"))
		{
			m_node->play();
		}
	}
}