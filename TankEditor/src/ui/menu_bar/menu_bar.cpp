#include <imgui/imgui.h>
#include "menu_bar.hpp"


namespace Tank::Editor
{
	void _MenuBar::drawUI()
	{
		if (ImGui::BeginMainMenuBar())
		{
			drawMainMenuBar();
			ImGui::EndMainMenuBar();
		}
	}
}