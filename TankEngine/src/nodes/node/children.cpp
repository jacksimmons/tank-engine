#include <nodes/node.hpp>
#include <log.hpp>


namespace Tank
{
	bool Node::setParent(Node *parent, std::optional<size_t> siblingIndex)
	{
		// Set the parent if it's different to our current
		if (m_parent == parent)
		{
			TE_CORE_TRACE(std::format("Parent is already {}, doing nothing.", parent->getName()));
			return false;
		}

		std::unique_ptr<Node> me = m_parent->disownChild(this);
		assert(me != nullptr);
		parent->addChild(std::move(me), siblingIndex);
		return true;
	}


	Node *Node::getParent() const noexcept
	{
		return m_parent;
	}


	void Node::addChild(std::unique_ptr<Node> child, std::optional<size_t> atIndex)
	{
		child->m_parent = this;
		m_childrenAwaitingAdopt.push_back(std::make_tuple(std::move(child), atIndex));
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


	bool Node::setSiblingIndex(size_t index)
	{
		if (index >= m_parent->getChildCount())
		{
			TE_CORE_ERROR(std::format("Couldn't set sibling index to {}, when parent only has {} children.", index, m_parent->getChildCount()));
			return false;
		}

		size_t currentIndex = getSiblingIndex();
		if (currentIndex == index)
		{
			TE_CORE_TRACE(std::format("Sibling index was already {}, doing nothing.", index));
			return true;
		}

		auto &vec = m_parent->m_children;

		// Left rotate
		if (currentIndex > index)
		{
			auto current = vec.begin() + currentIndex;
			auto target = vec.begin() + index;

			// Target index: 0
			//    { A, B, [X], Y, Z }
			// -> { [A, B, X], Y, Z } // Select range for rotation (exclusive on 3rd parameter)
			// -> { X, A, B, Y, Z }   // Done
			std::rotate(target, current, current + 1);
		}
		// Right rotate
		else
		{
			// The reverse iterator from start. Subtract from it to go towards the end.
			auto start = (vec.rbegin() + (vec.size() - 1));
			auto current = start - currentIndex;
			auto target = start - index;

			// Target index: 4
			//    { A, B, [X], Y, Z }
			// -> { A, B, [X, Y, Z] } // Select range for rotation (exclusive on 3rd parameter)
			// -> { A, B, Y, Z, X }   // Done
			std::rotate(target, current, current + 1);
		}
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