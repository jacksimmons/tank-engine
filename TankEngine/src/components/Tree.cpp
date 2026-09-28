#include <Log.h>
#include <ecs/Entity.h>
#include <static/GlmSerialise.h>
#include "Tree.h"


namespace Tank
{
	void TreeComponent::addChild(std::unique_ptr<Entity> child, std::optional<size_t> atIndex)
	{
		child->tree().m_parent = this->m_entity;
		m_childrenAwaitingAdopt.push_back(std::make_tuple(std::move(child), atIndex));
	}


	Entity *TreeComponent::getChild(const std::string &name) const
	{
		for (auto &child : m_children)
		{
			if (child->name() == name) return child.get();
		}

		// No child exists by this name.
		return nullptr;
	}


	Entity *TreeComponent::getChild(int index) const
	{
		if (0 <= index && index < m_children.size())
		{
			return m_children[index].get();
		}

		// Out of children list range.
		return nullptr;
	}


	std::unique_ptr<Entity> TreeComponent::disownChild(Entity *child)
	{
		std::unique_ptr<Entity> detached = nullptr;

		// Erase the child from m_children, if it is present there.
		// Also, if found, move the reference to `detached`, so we can return it.
		std::erase_if(m_children, [&detached, &child](std::unique_ptr<Entity> &ownedChild)
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


	std::vector<Entity*> TreeComponent::getSiblings() const
	{
		if (m_parent == nullptr) return {};
		return m_parent->tree().getChildrenOfType<Entity>();
	}


	Entity *TreeComponent::getSibling(const std::string &name) const
	{
		return m_parent->tree().getChild(name);
	}


	Entity *TreeComponent::getSibling(int index) const
	{
		return m_parent->tree().getChild(index);
	}


	int TreeComponent::getSiblingIndex() const
	{
		for (int i = 0; i < m_parent->getComponent<TreeComponent>().getChildCount(); i++)
		{
			if (&getSibling(i)->getComponent<TreeComponent>() == this)
				return i;
		}
		return -1;
	}


	int TreeComponent::getSiblingCount() const
	{
		return m_parent->tree().m_children.size();
	}


	bool TreeComponent::setSiblingIndex(size_t index)
	{
		if (index >= m_parent->getComponent<TreeComponent>().getChildCount())
		{
			TE_CORE_ERROR(std::format("Couldn't set sibling index to {}, when parent only has {} children.", index, m_parent->tree().getChildCount()));
			return false;
		}

		size_t currentIndex = getSiblingIndex();
		if (currentIndex == index)
		{
			TE_CORE_TRACE(std::format("Sibling index was already {}, doing nothing.", index));
			return true;
		}

		auto &vec = m_parent->tree().m_children;

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


	void TreeComponent::forEachDescendant(std::function<void(Entity *)> forEach, std::function<bool()> terminate)
	{
		std::stack<Entity *> entityStack;
		entityStack.push(m_entity);

		while (!entityStack.empty())
		{
			// Exit early if necessary
			if (terminate && terminate()) return;

			// Pop the top entity from the stack, and perform `forEach`.
			Entity *entity = entityStack.top();
			entityStack.pop();
			forEach(entity);

			// Add all its children to the stack.
			TreeComponent tree = entity->tree();
			for (int i = 0; i < tree.getChildCount(); i++)
			{
				entityStack.push(tree.getChild(i));
			}
		}
	}


	bool TreeComponent::setParent(Entity *parent, std::optional<size_t> siblingIndex)
	{
		// Set the parent if it's different to our current one
		if (parent == m_parent)
		{
			return false;
		}

		std::unique_ptr<Entity> disownedEntity = m_parent->tree().disownChild(m_entity);
		assert(disownedEntity != nullptr);
		parent->tree().addChild(std::move(disownedEntity), siblingIndex);
		return true;
	}


	std::string TreeComponent::getPath() const
	{
		if (m_parent)
		{
			return m_parent->tree().getPath() + "/" + m_entity->name();
		}

		return std::string("");
	}


	Entity *TreeComponent::childFromTree(std::vector<int> treeTraversal)
	{
		Entity *currentEntity = m_entity;
		while (!treeTraversal.empty())
		{
			int childIndex = treeTraversal[0];
			treeTraversal.erase(treeTraversal.begin());

			if (currentEntity)
			{
				currentEntity = currentEntity->tree().getChild(childIndex);
			}
			else
			{
				TE_CORE_ERROR("childFromTree: Tree traversal was not valid - attempted nullptr.getChild");
				return nullptr;
			}
		}

		return currentEntity;
	}


	std::vector<int> TreeComponent::treeFromChild(Entity *child)
	{
		Entity *currentChild = child;
		std::vector<int> treeTraversal;
		do
		{
			treeTraversal.emplace(treeTraversal.begin() + currentChild->tree().getSiblingIndex());
			currentChild = currentChild->tree().m_parent;

			if (!currentChild)
			{
				TE_CORE_ERROR("treeFromChild: Tree traversal was not valid - this was not a parent of child.");
				return std::vector<int>();
			}
		} while (currentChild != m_entity);

		return treeTraversal;
	}
}