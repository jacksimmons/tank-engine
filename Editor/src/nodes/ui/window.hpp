#pragma once
#include "imgui.h"
#include "nodes/node.hpp"


namespace Tank::Editor
{
	struct WindowOpts
	{
		ImGuiWindowFlags flags;
		bool closeable;
		bool autoScroll;
	};


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
		friend class _Profiler;
	private:
		_Window(const std::string &name, const WindowOpts &opts);
	protected:
		WindowOpts m_opts;
	public:
		virtual ~_Window() = default;

		virtual void draw() override;
		virtual void drawPanel() = 0;
	};
}
