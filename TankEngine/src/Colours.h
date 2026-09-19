#pragma once
#include <imgui.h>


namespace Tank
{
	namespace Colour
	{
		static constexpr ImColor NORMAL = ImColor(255, 255, 255);
		static constexpr ImColor TITLE = ImColor(0, 100, 255);
		static constexpr ImColor DISABLED = ImColor(128, 128, 128);
		static constexpr ImColor INFO = ImColor(0, 128, 0);
		static constexpr ImColor ERR = ImColor(128, 0, 0);

		static constexpr ImColor DIRECTORY = INFO;
		static constexpr ImColor FILE = NORMAL;
	}
}