#include <imgui/imgui.h>
#include "MenuBar.h"


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