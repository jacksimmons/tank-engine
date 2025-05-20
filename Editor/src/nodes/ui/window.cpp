#include "window.hpp"
#include "log.hpp"


namespace Tank::Editor
{
	_Window::_Window(const std::string &name, const ImGuiWindowFlags &flags, bool canBeClosed, bool autoScroll)
		: Node(name), m_canBeClosed(canBeClosed), m_autoScroll(autoScroll)
	{
		m_flags = flags | ImGuiWindowFlags_NoCollapse;
		m_isEditorControlled = true;
	}


	void _Window::draw()
	{
		bool open = true;
		ImGui::Begin(getName().c_str(), m_canBeClosed ? &open : nullptr, m_flags);
		{
			if (!m_canBeClosed || open) drawPanel();
			else
			{
				ImGui::End();
				destroy();
				return;
			}

			if (m_autoScroll)
				ImGui::SetScrollY(ImGui::GetScrollMaxY());

			ImGui::End();
		}
		Node::draw();
	}
}