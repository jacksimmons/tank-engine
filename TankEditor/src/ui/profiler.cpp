#include "profiler.hpp"
#include "static/time.hpp"
#include "colours.hpp"


namespace Tank::Editor
{
	const WindowOpts WINDOW_OPTS = {
		ImGuiWindowFlags_None,
		true,
		false
	};


	_Profiler::_Profiler(const std::string &name)
		: _Window(name, WINDOW_OPTS)
	{
	}


	void _Profiler::drawPanel()
	{
		ImGui::TextColored(Colour::TITLE, "Frame Delta");
		{
			ImGui::Text("All");
			ImGui::SameLine();
			ImGui::Text(std::format("{} (FPS: {})", Time::FrameDelta(), 1 / Time::FrameDelta()).c_str());
		}
	}
}