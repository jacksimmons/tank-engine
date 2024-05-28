#include <iostream>
#include "node.hpp"


namespace Tank
{
	Node::Node(std::string name)
	{
		m_name = name;
	}

	void Node::addChild(std::shared_ptr<Node> parent, std::shared_ptr<Node> child)
	{
		child->m_parent = parent;
		parent->m_children.push_back(std::move(child));
	}

	std::shared_ptr<Node> Node::getChild(std::string name) const
	{
		for (auto const &child : m_children)
		{
			if (child->getName() == name)
			{
				return child;
			}
		}

		// Return nullptr if no child was found.
		return std::shared_ptr<Node>();
	}

	void Node::draw() const
	{
		for (auto const &child : m_children)
		{
			child->draw();
		}
	}

	void Node::update()
	{
		if (m_enabled)
		{
			draw();
		}
	}
}