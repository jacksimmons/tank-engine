#pragma once
#include "imgui.h"
#include "nodes/ui/ui.hpp"

namespace Tank::Editor
{
	class _Window : public UI
	{
		// Classes which can access the constructor (permitted subclasses)
		friend class _Console;
		friend class _FileDialog;
		friend class _Hierarchy;
		friend class _Inspector;
		friend class _SceneView;
	private:
		_Window(const std::string &name, const ImGuiWindowFlags &flags = ImGuiWindowFlags_None, bool autoScroll = false);
	protected:
		ImGuiWindowFlags m_flags;
		bool m_autoScroll;
	public:
		virtual ~_Window() = default;

		virtual void drawUI() override;
		virtual void drawPanel() = 0;
	};
}
