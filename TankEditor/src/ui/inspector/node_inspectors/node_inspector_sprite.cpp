#include <imgui.h>
#include <colours.hpp>
#include <widget.hpp>
#include <nodes/sprite.hpp>
#include "../inspector.hpp"
#include "../../file_dialog.hpp"
#include "node_inspector.hpp"


namespace Tank::Editor
{
	/// <summary>
	/// Draws inspector section for a Sprite.
	/// </summary>
	void _NodeInspector<Sprite>::draw()
	{
		ImGui::TextColored(Colour::TITLE, "Sprite Texture");
		std::string texPath = m_node->getTexPath().string();
		Widget::textInput(
			"##Inspector_Sprite_Texture",
			texPath,
			[this](const std::string &modified)
			{
				if (fs::path{ modified } == m_node->getTexPath()) return;
				m_node->setTexPath(modified);
			},
			texPath
		);

		ImGui::SameLine();
		if (ImGui::SmallButton("..."))
		{
			std::string name = "Load Texture File";
			if (!m_inspector->getSibling(name))
			{
				auto fileDialog = std::unique_ptr<_FileDialog>(
					new _FileDialog(
						name,
						fs::current_path(),
						fs::path(texPath).parent_path(),
						_FileDialogTarget::File,
						[this](const fs::path &path)
						{
							// Only update the texture if user has selected a valid file
							if (!path.has_filename()) return;
							m_node->setTexPath(path);
						}
					));

				m_inspector->getParent()->addChild(std::move(fileDialog));
			}
		}
	}
}