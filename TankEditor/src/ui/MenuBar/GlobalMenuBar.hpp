#pragma once
#include "MenuBar.hpp"


namespace Tank::Editor
{
	class GlobalMenuBar_ final : public _MenuBar
	{
	private:
		void drawSystem();
	public:
		GlobalMenuBar_(EditorApp &editor) : _MenuBar(editor) {}

		void drawMainMenuBar() override;
	};
}