#include "window.hpp"


namespace Tank::Editor
{
	_Window::_Window(const std::string &name, const ImGuiWindowFlags &flags, bool autoScroll) : UI(name), m_autoScroll(autoScroll)
	{
		m_flags = flags | ImGuiWindowFlags_NoCollapse;
	}


	void _Window::drawUI()
	{
		bool open;
		ImGui::Begin(getName().c_str(), &open, m_flags);

		if (open) drawPanel();
		if (m_autoScroll)
			ImGui::SetScrollY(ImGui::GetScrollMaxY());

		ImGui::End();
	}
}