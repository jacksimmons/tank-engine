#pragma once
#include "imgui.h"
#include "nodes/interfaces/editor_only.hpp"


namespace Tank::Editor
{
	/// <summary>
	/// Base class to render an ImGui window in the Editor.
	/// </summary>
	class _Window : public Node
	{
		// Classes which can access the constructor (permitted subclasses)
		friend class _Console;
		friend class _FileDialog;
		friend class _Hierarchy;
		friend class _Inspector;
		friend class _SceneView;
	private:
		_Window(const std::string &name, const ImGuiWindowFlags &flags = ImGuiWindowFlags_None, bool canBeClosed = true, bool autoScroll = false);
		bool m_canBeClosed;
	protected:
		ImGuiWindowFlags m_flags;
		bool m_autoScroll;
	public:
		virtual ~_Window() = default;

		virtual void draw() override;
		virtual void drawPanel() = 0;
	};
}
