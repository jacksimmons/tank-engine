#include "window.hpp"
#include "log.hpp"


namespace Tank
{
	_Window::_Window(const std::string &name, const WindowOpts &opts)
		: Node(name), m_opts(opts)
	{
		m_opts.flags |= ImGuiWindowFlags_NoCollapse;
		m_isEditorControlled = true;
	}


	void _Window::draw()
	{
		bool open = true;
		ImGui::Begin(getName().c_str(), m_opts.closeable ? &open : nullptr, m_opts.flags);
		{
			if (!m_opts.closeable || open) drawPanel();
			else
			{
				ImGui::End();
				destroy();
				return;
			}

			if (m_opts.autoScroll)
				ImGui::SetScrollY(ImGui::GetScrollMaxY());

			ImGui::End();
		}
		Node::draw();
	}
}