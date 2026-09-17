#pragma once
#include "MenuBar.hpp"
#include <nodes/ui/UiNode.hpp>


namespace Tank::Editor
{
	class EditorApp;
	class ProjectMenuBar_ final : public _MenuBar
	{
	private:
		void drawFile();
		void drawView();

		bool spawnerMenuItem(const std::string &nodeName);
	public:
		ProjectMenuBar_(EditorApp &editor) : _MenuBar(editor) {}
		
		void drawMainMenuBar() override;
	};
}