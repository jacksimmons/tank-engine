#include "editor_node.hpp"


namespace Tank::Editor
{
	EditorNode::EditorNode(const std::string &name) : Node(name)
	{
	}

	void EditorNode::startup()
	{
		//setEnabled(false);
	}

	void EditorNode::shutdown()
	{
		//setEnabled(true);
	}
}