#include <imgui/imgui.h>
#include "MenuBar.hpp"


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