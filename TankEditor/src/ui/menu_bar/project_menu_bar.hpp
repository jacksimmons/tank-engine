#pragma once
#include "nodes/node.hpp"


namespace Tank::Reflect
{
	class NodeFactory;
}
namespace Tank::Editor
{
	class EditorApp;
	class ProjectMenuBar final : public Node
	{
	private:
		EditorApp &m_editor;

		void drawFile();
		void drawView();

		bool spawnerMenuItem(const std::string &nodeName);
	public:
		virtual void draw() override;

		ProjectMenuBar(EditorApp &editor) : Node("ProjectMenuBar"), m_editor(editor) {};
	};
}