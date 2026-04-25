#pragma once
#include "menu_bar.hpp"


namespace Tank::Editor
{
	class _GlobalMenuBar final : public _MenuBar
	{
	private:
		void drawSystem();
	public:
		_GlobalMenuBar(EditorApp &editor) : _MenuBar(editor) {}

		void drawMainMenuBar() override;
	};
}