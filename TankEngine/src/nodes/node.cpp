#include "node.hpp"
#include "log.hpp"
#include "transform.hpp"


namespace Tank
{
	Node::Node(const std::string &name)
	{
		m_type = "Node";
		m_name = name;
		m_transform = std::make_unique<Transform>(this);
		m_parent = nullptr;
	}


	std::string Node::getPath() const
	{
		if (m_parent)
		{
			return m_parent->getPath() + "/" + m_name;
		}

		return std::string("");
	}


	void Node::addChild(std::unique_ptr<Node> child)
	{
		child->m_parent = this;
		m_childrenAwaitingAdopt.push_back(std::move(child));
	}

	Node *Node::getChild(std::string name) const
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


	Node *Node::childFromTree(std::vector<int> treeTraversal)
	{
		Node *currentNode = this;
		while (!treeTraversal.empty())
		{
			int childIndex = treeTraversal[0];
			treeTraversal.erase(treeTraversal.begin());
			
			if (currentNode)
			{
				currentNode = currentNode->getChild(childIndex);
			}
			else
			{
				TE_CORE_ERROR("childFromTree: Tree traversal was not valid - attempted nullptr.getChild");
				return nullptr;
			}
		}

		return currentNode;
	}


	std::vector<int> Node::treeFromChild(Node *child)
	{
		Node *currentChild = child;
		std::vector<int> treeTraversal;
		do
		{
			treeTraversal.emplace(treeTraversal.begin() + currentChild->getSiblingIndex());
			currentChild = currentChild->m_parent;

			if (!currentChild)
			{
				TE_CORE_ERROR("treeFromChild: Tree traversal was not valid - this was not a parent of child.");
				return std::vector<int>();
			}
		} while (currentChild != this);

		return treeTraversal;
	}


	void Node::draw()
	{
	}


	void Node::startup()
	{
		if (!m_enabled) return;
		if (m_started) return;
		m_started = true;

		//for (auto const &script : m_scripts)
		//{
		//	script->setEnabled(true);
		//	script->startup();
		//}

		for (auto const &child : m_children)
		{
			child->startup();
		}
	}


	void Node::shutdown()
	{
		if (!m_enabled) return;
		if (!m_started) return;
		m_started = false;

		//for (auto const &script : m_scripts)
		//{
		//	script->setEnabled(false);
		//	script->shutdown();
		//}

		for (auto const &child : m_children)
		{
			child->shutdown();
		}
	}


	void Node::preupdate()
	{
		// Disown all children waiting to be disowned
		for (Node *toDisown : m_childrenAwaitingDisown)
		{
			m_children.erase(m_children.begin() + toDisown->getSiblingIndex());
		}
		m_childrenAwaitingDisown.clear();

		// Adopt all children waiting to be adopted
		for (auto it = m_childrenAwaitingAdopt.begin(); it != m_childrenAwaitingAdopt.end(); ++it)
		{
			m_children.push_back(std::move(*it));
		}
		m_childrenAwaitingAdopt.clear();
	}


	void Node::update()
	{
		if (!m_enabled) return;
		if (m_visible) draw();

		//for (auto const &script : m_scripts)
		//{
		//	script->update();
		//}

		preupdate();

		// Recursively update
		for (auto it = m_children.begin(); it != m_children.end(); ++it)
		{
			(*it)->update();
		}
	}
}


json Tank::Node::serialise()
{
	json serialised;
	serialised["name"] = m_name;
	serialised["type"] = m_type;
	serialised["enabled"] = m_enabled;
	serialised["visible"] = m_visible;
	serialised["transform"] = Transform::serialise(m_transform.get());
	return serialised;
}

void Tank::Node::deserialise(const json &serialised)
{
	setName(serialised["name"]);
	setEnabled(serialised["enabled"]);
	setVisibility(serialised["visible"]);
	Transform::deserialise(serialised["transform"], getTransform());
}
