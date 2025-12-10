#include "editor_only.hpp"


namespace Tank
{
	IEditorOnly::IEditorOnly(const std::string &name) : Node(name)
	{
	}


	void IEditorOnly::startup()
	{
		setVisibility(false);
		// @todo Find a better solution than hard-coding child visibility
		for (std::unique_ptr<Node> &node : m_children)
		{
			node->setVisibility(false);
		}
		Node::startup();
	}


	void IEditorOnly::shutdown()
	{
		setVisibility(true);
		// @todo Find a better solution than hard-coding child visibility
		for (std::unique_ptr<Node> &node : m_children)
		{
			node->setVisibility(true);
		}
		Node::shutdown();
	}
}