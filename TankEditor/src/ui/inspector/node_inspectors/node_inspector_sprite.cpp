#include <imgui.h>
#include <colours.hpp>
#include <widget.hpp>
#include <nodes/sprite.hpp>
#include <events/event_manager.hpp>
#include "../inspector.hpp"
#include "ui/file_dialog.hpp"
#include "node_inspector.hpp"


const std::string g_name = "Load Texture File";


namespace Tank::Editor
{
	/// <summary>
	/// Draws inspector section for a Sprite.
	/// </summary>
	template <>
	void _NodeInspector<Sprite>::draw()
	{
		ImGui::TextColored(Colour::TITLE, "Sprite Texture");
		fs::path texPath = m_node->getTexPath();
		Widget::textInput(
			"##Inspector_Sprite_Texture",
			texPath.string(),
			[this](const std::string &modified)
			{
				if (fs::path{ modified } == m_node->getTexPath()) return;
				m_node->setTexPath(modified);
			},
			texPath.string()
		);

		ImGui::SameLine();
		if (ImGui::SmallButton("..."))
		{
			if (!m_inspector->getSibling(g_name))
			{
				auto fileDialog = std::unique_ptr<_FileDialog>(
					new _FileDialog(
						g_name,
						fs::current_path(),
						texPath.parent_path(),
						_FileDialogTarget::File,
						[this](const fs::path &path)
						{
							m_node->setTexPath(path);
						}
					));

				m_inspector->addChild(std::move(fileDialog));
			}
		}
	}
}