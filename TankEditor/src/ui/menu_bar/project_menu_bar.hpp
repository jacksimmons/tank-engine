#pragma once
#include <nodes/ui/ui_node.hpp>


namespace Tank::Reflect
{
	class NodeFactory;
}
namespace Tank::Editor
{
	class EditorApp;
	class _ProjectMenuBar final : public UINode
	{
	private:
		EditorApp &m_editor;

		void drawFile();
		void drawView();

		bool spawnerMenuItem(const std::string &nodeName);
	public:
		_ProjectMenuBar(EditorApp &editor) : UINode("ProjectMenuBar"), m_editor(editor)
		{
			m_isEditorControlled = true;
		}
		
		void drawUI() override;
	};
}