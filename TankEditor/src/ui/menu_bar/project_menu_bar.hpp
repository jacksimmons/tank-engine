#pragma once
#include <nodes/ui/ui_node.hpp>


namespace Tank::Reflect
{
	class NodeFactory;
}
namespace Tank::Editor
{
	class EditorApp;
	class ProjectMenuBar final : public UINode
	{
	private:
		EditorApp &m_editor;

		void drawFile();
		void drawView();

		bool spawnerMenuItem(const std::string &nodeName);
	public:
		ProjectMenuBar(EditorApp &editor) : UINode("ProjectMenuBar"), m_editor(editor) {};
		
		void drawUI() override;
	};
}