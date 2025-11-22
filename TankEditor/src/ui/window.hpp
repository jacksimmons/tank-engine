#pragma once
#include "imgui.h"
#include "nodes/node.hpp"


namespace Tank
{
	namespace Editor
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
		class _Window : public Node
		{
		protected:
			_Window(const std::string &name, const WindowOpts &opts);
			WindowOpts m_opts;
		public:
			virtual ~_Window() = default;

			/// <summary>
			/// Inherited draw method from Node. Calls drawPanel.
			/// </summary>
			virtual void draw() override;

			/// <summary>
			/// [ABSTRACT]
			/// All draw logic for the window goes in here.
			/// This function is called between an ImGui::Begin
			/// and an ImGui::End.
			/// </summary>
			virtual void drawPanel() = 0;
		};
	}
}
