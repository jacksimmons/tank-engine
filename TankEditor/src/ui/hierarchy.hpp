#pragma once
#include <type_traits>
#include "ui/window.hpp"
#include "events/event.hpp"


namespace Tank::Editor
{
	class _Inspector;
	class _Hierarchy final : public _Window
	{
		friend class EditorApp;
		
		// Fields
	private:
		Node *m_currentRoot;
		bool m_showEditorHierarchy;
		std::unique_ptr<Event<Node*>> m_onNodeSelected;
		std::unique_ptr<Event<Node*>> m_onNodeDeleted;

		// Methods
		/// <summary>
		/// Draws a tree node for the node provided, then calls itself for each
		/// of its children. Draws a leaf instead if no children.
		/// </summary>
		void drawTreeNode(Node *node, int *count);

		/// <summary>
		/// Handles drawing for node context menus. A node context menu is
		/// displayed when a node is right-clicked in the hierarchy.
		/// This allows the user to Delete Nodes (leading to *node
		/// = nullptr).
		/// Returns true if the node survives, false if it is destroyed.
		/// </summary>
		bool drawNodeContextMenu(Node *node);

		/// <summary>
		/// Adds a created node as a child of parent.
		/// </summary>
		void addNewNode(Node *parent, Node *heapAllocatedNode) const;

		_Hierarchy(const std::string &name = "Hierarchy");
	protected:
		/// <summary>
		/// Generates buttons for all children of the current node, at a given
		/// indentation depth (based on the generation depth).
		/// </summary>
		virtual void drawPanel() override;
	public:
		void handleOnNodeSelected(EventHandler<Node*> handler) { m_onNodeSelected->registerHandler(handler); }
		void handleOnNodeDeleted(EventHandler<Node*> handler) { m_onNodeDeleted->registerHandler(handler); }
	};
}
