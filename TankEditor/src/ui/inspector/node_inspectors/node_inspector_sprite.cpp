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
		std::string texPath = Res::encode(m_node->getTexPath());
		Widget::textInput(
			"##Inspector_Sprite_Texture",
			texPath,
			[this, &texPath](const std::string &modified)
			{
				if (modified == texPath) return;
				m_node->setTexPath(Resource::decode(modified));
			},
			texPath
		);

		ImGui::SameLine();
		if (ImGui::SmallButton("..."))
		{
			if (!m_inspector->getSibling(g_name))
			{
				auto fileDialog = std::unique_ptr<_FileDialog>(
					new _FileDialog(
						g_name,
						Resource::getProjPath(),
						Resource::getProjPath(),
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