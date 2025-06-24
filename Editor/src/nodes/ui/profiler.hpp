#pragma once
#include "nodes/ui/window.hpp"


namespace Tank::Editor
{
	class _Profiler : public _Window
	{
		friend class EditorApp;
	private:
		_Profiler(const std::string &name = "Profiler");
	public:
		virtual void drawPanel() override;
	};
}