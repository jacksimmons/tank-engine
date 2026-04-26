#pragma once
#include "ui/window.hpp"


namespace Tank::Editor
{
	class Profiler_ : public _Window
	{
		friend class EditorApp;
		friend class ProjectMenuBar_;
	private:
		Profiler_(const std::string &name = "Profiler");
	public:
		virtual void drawPanel() override;
	};
}