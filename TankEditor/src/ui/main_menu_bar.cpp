#include "imgui.h"
#include "main_menu_bar.hpp"
#include "nodes/scene.hpp"


namespace Tank::Editor
{
	_MainMenuBar::_MainMenuBar(const std::string &name, const std::vector<Tab> &tabs)
		: Node(name), m_tabs(tabs)
	{
		m_isEditorControlled = true;
	}


	void _MainMenuBar::draw()
	{
		// Start main menu bar.
		if (ImGui::BeginMainMenuBar())
		{
			// For each tab, begin a menu.
			for (const Tab &tab : m_tabs)
			{
				if (ImGui::BeginMenu(tab.name.c_str(), tab.getEnabled(tab.name)))
				{
					// For each tab item, begin a menu item.
					for (const TabItem &item : tab.items)
					{
						bool isSelected = false;
						ImGui::MenuItem(item.name.c_str(), "", &isSelected, item.getEnabled(item.name));
						if (isSelected)
						{
							item.onSelect();
						}
					}

					ImGui::EndMenu();
				}
			}

			ImGui::EndMainMenuBar();
		}
	}
}