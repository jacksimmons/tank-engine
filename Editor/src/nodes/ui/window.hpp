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

			virtual void draw() override;
			virtual void drawPanel() = 0;
		};
	}
}
