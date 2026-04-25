#pragma once
#include "menu_bar.hpp"
#include <nodes/ui/ui_node.hpp>


namespace Tank::Editor
{
	class EditorApp;
	class _ProjectMenuBar final : public _MenuBar
	{
	private:
		void drawFile();
		void drawView();

		bool spawnerMenuItem(const std::string &nodeName);
	public:
		_ProjectMenuBar(EditorApp &editor) : _MenuBar(editor) {}
		
		void drawMainMenuBar() override;
	};
}