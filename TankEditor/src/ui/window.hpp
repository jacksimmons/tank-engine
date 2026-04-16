#pragma once
#include "imgui.h"
#include <nodes/ui/ui_node.hpp>


namespace Tank::Editor
{
	struct WindowOpts
	{

		ImGuiWindowFlags flags;
		bool closeable;
		bool autoScroll;
	};


	/// <summary>
	/// Base class to render an ImGui window.
	/// </summary>
	class _Window : public UINode
	{
	protected:
		_Window(const std::string &name, const WindowOpts &opts);
		WindowOpts m_opts;
	public:
		virtual ~_Window() = default;

		/// <summary>
		/// Inherited draw method from Node. Calls drawPanel.
		/// </summary>
		virtual void drawUI() override;

		/// <summary>
		/// [ABSTRACT]
		/// All draw logic for the window goes in here.
		/// This function is called between an ImGui::Begin
		/// and an ImGui::End.
		/// </summary>
		virtual void drawPanel() = 0;
	};
}
