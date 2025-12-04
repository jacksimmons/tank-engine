#pragma once
#include <events/event.hpp>
#include "nodes/interfaces/serialisable.hpp"
#include "nodes/interfaces/scriptable.hpp"


namespace Tank
{
	class Transform;

	/// <summary>
	/// An object which exists in the Node hierarchy. It has a parent (or is the root),
	/// and any number of children.
	/// 
	/// Nodes can be serialised and deserialised.
	/// </summary>
	class TANK_API Node : public ISerialisable, public IScriptable
	{
	public:
		virtual json serialise();
		virtual void deserialise(const json &serialised);

	private:
		std::string m_name;
		/// <summary>
		/// If false, Node::update isn't invoked for this Node and all children.
		/// </summary>
		bool m_enabled = true;
		/// <summary>
		/// If false, Node::draw isn't invoked for this Node only.
		/// </summary>
		bool m_visible = true;

		std::vector<Node*> m_childrenAwaitingDisown;
		std::vector<std::unique_ptr<Node>> m_childrenAwaitingAdopt;
	protected:
		std::string m_type;
		std::unique_ptr<Transform> m_transform;
		Node *m_parent;
		std::vector<std::unique_ptr<Node>> m_children;
		bool m_started = false;

		/// <summary>
		/// If true, this is controlled by the editor: the Hierarchy and Inspector are
		/// unable to edit this node.
		/// </summary>
		bool m_isEditorControlled = false;
	public:
		Node(const std::string &name = "Node");
		virtual ~Node() = default;

		constexpr const std::string& getType() const noexcept { return m_type; }

		constexpr void setEnabled(bool enabled) noexcept { m_enabled = enabled; }
		constexpr bool getEnabled() const noexcept { return m_enabled; }

		constexpr void setVisibility(bool visible) noexcept { m_visible = visible; }
		constexpr bool getVisibility() const noexcept { return m_visible; }

		void setName(const std::string &name) noexcept { m_name = name; }
		constexpr const std::string& getName() const noexcept { return m_name; }
		std::string getPath() const;

		constexpr bool isEditorControlled() const { return m_isEditorControlled; }

		constexpr void setParent(Node *parent) noexcept { m_parent = parent; }
		constexpr Node *getParent() const noexcept { return m_parent; }

		Transform *getTransform() const;

		size_t getChildCount() const noexcept { return m_children.size(); }
		typedef std::vector<std::unique_ptr<Node>>::iterator iterator;
		typedef std::vector<std::unique_ptr<Node>>::const_iterator const_iterator;
		iterator begin() noexcept { return m_children.begin(); }
		iterator end() noexcept { return m_children.end(); }
		const_iterator begin() const noexcept { return m_children.begin(); }
		const_iterator end() const noexcept { return m_children.end(); }

		// Add an existing child.
		void addChild(std::unique_ptr<Node> child);
		// Get first child whose name matches the provided name.
		Node *getChild(std::string name) const;
		// Get child by index.
		Node *getChild(int index) const;
		// Get first sibling whose name matches the provided name.
		Node *getSibling(std::string name) const { return getParent()->getChild(name); }
		// Get sibling by index.
		Node *getSibling(int index) const { return getParent()->getChild(index); }
		// Get parent's child index for this node.
		int getSiblingIndex() const;

		void forEachDescendant(std::function<void(Node *)> forEach, std::function<bool()> terminate = nullptr);

		/// <summary>
		/// Starting at this node, traverse through descendant tree by the instruction of a tree-traversal
		/// array.
		/// 
		/// Element at index n denotes which sibling to continue down, at a depth of n in the descendant tree,
		/// where a depth of 0 is the depth of getChild.
		/// { 1, 0 } = getChild(1)->getChild(0);
		/// </summary>
		Node *childFromTree(std::vector<int> treeTraversal);
		/// <summary>
		/// Builds a descendant tree traversal up to `this`, from a child of `this`.
		/// </summary>
		std::vector<int> treeFromChild(Node *child);

		virtual void startup();
		virtual void preupdate();
		virtual void draw();
		virtual void update();
		virtual void shutdown();
		void destroy() { m_parent->m_childrenAwaitingDisown.push_back(this); };
	};
}