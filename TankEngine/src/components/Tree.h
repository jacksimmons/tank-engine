#pragma once
#include <glm/gtx/quaternion.hpp>
#include <serialisation/Serialisation.h>


namespace Tank
{
	typedef std::tuple<std::unique_ptr<GameEntity>, std::optional<size_t>> Adoption;


	/// @brief Positions an Entity in the scene tree.
	struct TreeComponent
	{
		friend class Entity;
		friend class GameEntity;
	private:
		GameEntity *m_entity;
		GameEntity *m_parent;
		std::vector<std::unique_ptr<GameEntity>> m_children;
		std::vector<GameEntity *> m_childrenAwaitingDisown;
		std::vector<Adoption> m_childrenAwaitingAdopt;
	public:
		TreeComponent() = default;
		TreeComponent(GameEntity *entity, GameEntity *parent = nullptr)
			: m_entity(entity), m_parent(parent) {}
		TreeComponent(const TreeComponent &) = default;

		bool setParent(TreeComponent *parent, std::optional<size_t> siblingIndex);
		/// @brief Sets the parent of this Node.
		/// The parent controls ownership and this Node will be stored in parent's m_children.
		/// @param parent 
		/// @param siblingIndex If provided, gets added by insert rather than push_back.
		/// @return Whether the operation was successful (parent changed).
		bool setParent(GameEntity *parent, std::optional<size_t> siblingIndex = std::nullopt);
		GameEntity &getParent() const noexcept { return *m_parent; }

		std::string getPath() const;
		size_t getChildCount() const noexcept { return m_children.size(); }

		typedef std::vector<std::unique_ptr<GameEntity>>::iterator iterator;
		typedef std::vector<std::unique_ptr<GameEntity>>::const_iterator const_iterator;
		iterator begin() noexcept { return m_children.begin(); }
		iterator end() noexcept { return m_children.end(); }
		const_iterator begin() const noexcept { return m_children.begin(); }
		const_iterator end() const noexcept { return m_children.end(); }

		// Add an existing child.

		/// @brief Adds a node as a child, at the end of m_children by default.
		/// @param child 
		/// @param atIndex If provided, inserts the child at the index instead.
		void addChild(std::unique_ptr<GameEntity> child, std::optional<size_t> atIndex = std::nullopt);

		/// @brief Gets a child by name.
		/// @param name 
		/// @return The child, or nullptr if unsuccessful.
		GameEntity *getChild(const std::string &name) const;

		/// @brief Gets a child by sibling index.
		/// @param index Sibling index (index in m_children).
		/// @return The child, or nullptr if unsuccessful.
		GameEntity *getChild(int index) const;

		/// @brief Disown a child, removing it from the tree.
		/// @param child
		/// @return An owning reference to the child.
		std::unique_ptr<GameEntity> disownChild(GameEntity *child);

		// Get all children of type (if any).
		template <class T>
		std::vector<T *> getChildrenOfType() const
		{
			std::vector<T *> results;
			for (const auto &child : m_children)
			{
				if (T *t = dynamic_cast<T *>(child.get()))
				{
					results.push_back(t);
				}
			}

			return results;
		}

		std::vector<GameEntity *> getSiblings() const;
		GameEntity *getSibling(const std::string &name) const; // By name
		GameEntity *getSibling(int index) const; // By index

		// Get parent's child index for this node.
		int getSiblingIndex() const;
		int getSiblingCount() const;

		/// @brief Sets the index of this node in its parent's m_children.
		/// @param index 
		/// @return Whether it was successful.
		bool setSiblingIndex(size_t index);

		void forEachDescendant(std::function<void(GameEntity *)> forEach, std::function<bool()> terminate = nullptr);

		/// <summary>
		/// Starting at this node, traverse through descendant tree by the instruction of a tree-traversal
		/// array.
		/// 
		/// Element at index n denotes which sibling to continue down, at a depth of n in the descendant tree,
		/// where a depth of 0 is the depth of getChild.
		/// { 1, 0 } = getChild(1)->getChild(0);
		/// </summary>
		GameEntity *childFromTree(std::vector<int> treeTraversal);
		/// <summary>
		/// Builds a descendant tree traversal up to `this`, from a child of `this`.
		/// </summary>
		std::vector<int> treeFromChild(GameEntity *child);
	};


	template <>
	json serialise<TreeComponent>(TreeComponent *);
	template <>
	void deserialise<TreeComponent>(const json &, TreeComponent *);
}