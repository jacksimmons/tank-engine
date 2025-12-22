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
		std::string audioPath = Res::encode(m_node->getAudioPath());
		Widget::textInput(
			"##Inspector_Audio_File",
			audioPath,
			[this, &audioPath](const std::string &modified)
			{
				if (modified == audioPath) return;
				m_node->setAudioPath(Resource::decode(modified));
				m_node->updateSound();
			},
			audioPath
		);

		ImGui::SameLine();
		if (ImGui::SmallButton("..."))
		{
			if (!m_inspector->getSibling(g_name))
			{
				auto fileDialog = std::make_unique<_FileDialog>(
					g_name,
					Resource::getProjPath(),
					Resource::getProjPath(),
					_FileDialogTarget::File,
					[this](const fs::path &path)
					{
						m_node->setAudioPath(path);
						m_node->updateSound();
					}
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