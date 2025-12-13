#include "editor_only.hpp"


namespace Tank
{
	IEditorOnly::IEditorOnly(const std::string &name) : Node(name)
	{
	}


	void IEditorOnly::startup()
	{
		Visible = false;
		// @todo Find a better solution than hard-coding child visibility
		for (std::unique_ptr<Node> &node : m_children)
		{
			node->Visible = false;
		}
		Node::startup();
	}


	void IEditorOnly::shutdown()
	{
		Visible = true;
		// @todo Find a better solution than hard-coding child visibility
		for (std::unique_ptr<Node> &node : m_children)
		{
			node->Visible = true;
		}
		Node::shutdown();
	}
}