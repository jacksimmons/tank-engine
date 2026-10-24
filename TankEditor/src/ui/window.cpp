#include "Window.h"
#include "Log.h"


namespace Tank::Editor
{
	_Window::_Window(entt::entity handle, Scene *ecs, WindowOpts opts)
		: EditorEntity(handle, ecs), m_opts(opts)
	{
		m_opts.flags |= ImGuiWindowFlags_NoCollapse;
		m_isEditorControlled = true;
	}


	void _Window::drawUI()
	{
		bool open = true;
		ImGui::Begin(name().c_str(), m_opts.closeable ? &open : nullptr, m_opts.flags);
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
	}
}
