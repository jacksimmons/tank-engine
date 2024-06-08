#include <iostream>

#include "node.hpp"
#include "transform.hpp"


namespace Tank
{
	Node::Node(std::string name)
	{
		m_name = name;
		m_transform = std::make_unique<Transform>();
	}

	void Node::addChild(std::unique_ptr<Node> child)
	{
		child->m_parent = this;
		m_children.push_back(std::move(child));
	}

	Node *Node::getChild(std::string name) const
	{
		for (int i = 0; i < getChildCount(); i++)
		{
			Node *child = getChild(i);
			if (child->getName() == name)
			{
				return child;
			}
		}

		// No child exists by this name.
		return nullptr;
	}

	Node *Node::getChild(int index) const
	{
		if (0 <= index && index < m_children.size())
		{
			return m_children[index].get();
		}
		
		// Out of children list range.
		return nullptr;
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