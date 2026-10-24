#pragma once
#include "ui/Window.h"


namespace Tank::Editor
{
	class Profiler_ : public _Window
	{
		friend class EditorApp;
		friend class ProjectMenuBar_;
	private:
		Profiler_(entt::handle handle, Scene *ecs);
	public:
		virtual void drawPanel() override;
	};
}