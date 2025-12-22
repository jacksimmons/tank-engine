#pragma once
#include <utils/getset.hpp>
#include "nodes/interfaces/serialisable.hpp"
#include "nodes/interfaces/scriptable.hpp"


namespace Tank
{
	class Transform;
	class KeyInput;
	class Script;

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
		std::unique_ptr<KeyInput> m_keyInput;
	protected:
		std::string m_type;
		Node *m_parent = nullptr;
		std::unique_ptr<Transform> m_transform;
		std::vector<std::unique_ptr<Node>> m_children;
		std::vector<std::unique_ptr<Script>> m_scripts;
		bool m_started = false;

		/// <summary>
		/// If true, this is controlled by the editor: the Hierarchy and Inspector are
		/// unable to edit this node.
		/// </summary>
		bool m_isEditorControlled = false;
	public:
		Node(const std::string &name = "Node");
		virtual ~Node();

		GetSet<bool> Enabled = m_enabled;
		GetSet<bool> Visible = m_visible;
		Get<bool> IsEditorControlled = m_isEditorControlled;

		void setName(const std::string &name) noexcept;
		constexpr const std::string& getName() const noexcept { return m_name; }
		
		void setParent(Node *parent) noexcept { m_parent = parent; }
		Node *getParent() const noexcept { return m_parent; }

		std::string getPath() const;
		KeyInput *getKeyInput() const;
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
		Node *getChild(const std::string &name) const;
		// Get child by index.
		Node *getChild(int index) const;
		// Get first child of type (if any).
		template <class T>
		std::vector<T*> getChildrenOfType() const
		{
			std::vector<T*> results;
			for (const auto &child : m_children)
			{
				if (T *t = dynamic_cast<T*>(child.get()))
				{
					results.push_back(t);
				}
			}

			return results;
		}

		// Get all siblings.
		template <class T>
		std::vector<T*> getSiblingsOfType() const
		{
			if (m_parent == nullptr) return std::vector<T*>();
			return m_parent->getChildrenOfType<T>();
		}
		// Get first sibling whose name matches the provided name.
		Node *getSibling(const std::string &name) const { return getParent()->getChild(name); }
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

		void addScript(std::unique_ptr<Script>);
		std::vector<Res> getScriptPaths();

		virtual void startup();
		virtual void preupdate();
		virtual void draw();
		virtual void update();
		virtual void shutdown();
		void destroy() { m_parent->m_childrenAwaitingDisown.push_back(this); };
	};
}