#include <nodes/node.hpp>


namespace Tank
{
	void Node::setParent(Node *parent)
	{
		std::unique_ptr<Node> me = m_parent->disownChild(this);
		assert(me != nullptr);

		parent->addChild(std::move(me));
	}


	Node *Node::getParent() const noexcept
	{
		return m_parent;
	}


	void Node::addChild(std::unique_ptr<Node> child)
	{
		child->m_parent = this;
		m_childrenAwaitingAdopt.push_back(std::move(child));
	}


	Node *Node::getChild(const std::string &name) const
	{
		for (auto &child : m_children)
		{
			if (child->getName() == name) return child.get();
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


	std::unique_ptr<Node> Node::disownChild(Node *child)
	{
		std::unique_ptr<Node> detached = nullptr;

		// Erase the child from m_children, if it is present there.
		// Also, if found, move the reference to `detached`, so we can return it.
		std::erase_if(m_children, [&detached, &child](std::unique_ptr<Node> &ownedChild)
		{
			if (child == ownedChild.get())
			{
				detached = std::move(ownedChild);
				return true;
			}
			return false;
		});

		return detached;
	}


	int Node::getSiblingIndex() const
	{
		for (int i = 0; i < m_parent->getChildCount(); i++)
		{
			if (getSibling(i) == this)
				return i;
		}
		return -1;
	}


	void Node::forEachDescendant(std::function<void(Node *)> forEach, std::function<bool()> terminate)
	{
		std::stack<Node *> nodeStack;
		nodeStack.push(this);

		while (!nodeStack.empty())
		{
			// Exit early if necessary
			if (terminate && terminate()) return;

			// Pop the top node from the stack, and perform `forEach`.
			Node *node = nodeStack.top();
			nodeStack.pop();
			forEach(node);

			// Add all its children to the stack.
			for (int i = 0; i < node->getChildCount(); i++)
			{
				nodeStack.push(node->getChild(i));
			}
		}
	}
}