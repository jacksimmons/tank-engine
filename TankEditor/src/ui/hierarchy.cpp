#include <typeinfo>
#include <imgui.h>
#include "log.hpp"
#include "colours.hpp"
#include <events/event_manager.hpp>
#include "nodes/camera.hpp"
#include "nodes/light.hpp"
#include "nodes/scene.hpp"
#include "nodes/cube_map.hpp"
#include "nodes/model.hpp"
#include "nodes/sprite.hpp"
#include "nodes/ui/text.hpp"
#include "ui/console.hpp"
#include "ui/hierarchy.hpp"
#include "nodes/physics/collider.hpp"
#include "shapes/collision_sphere.hpp"


namespace Tank::Editor
{
	const WindowOpts WINDOW_OPTS = {
		ImGuiWindowFlags_None,
		true,
		false
	};


	Hierarchy_::Hierarchy_(const std::string &name)
		: _Window(name, WINDOW_OPTS)
	{
		m_showEditorHierarchy = false;
		m_currentRoot = Tank::Scene::getActiveScene();
	}


	void Hierarchy_::drawPanel()
	{
		if (m_showEditorHierarchy)
		{
			while (true)
			{
				Node *parent = m_currentRoot->getParent();
				if (parent) m_currentRoot = parent;
				else break;
			}
		}
		else
		{
			m_currentRoot = Tank::Scene::getActiveScene();
		}

		int count = 0;
		drawTreeNode(m_currentRoot, &count);
	}


	/// <summary>
	/// Generates buttons for all children of the current node, at a given
	/// indentation depth (based on the generation depth).
	/// </summary>
	void Hierarchy_::drawTreeNode(Node *node, int *count)
	{
		// Base case.
		if (!node) return;

		size_t childCount = node->getChildCount();

		// Determine if leaf node.
		ImGuiTreeNodeFlags flags = 0;
		if (childCount == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;

		// Add node name to the tree, clicking on this node will set `nodeExpanded` to true if not a leaf node.
		ImVec4 nodeNameCol = Colour::NORMAL;
		if (!node->Enabled()) nodeNameCol = Colour::DISABLED;
		if (node->IsEditorControlled()) nodeNameCol = Colour::ERR;

		drawSiblingSeparator(node);
		ImGui::PushStyleColor(ImGuiCol_Text, nodeNameCol);
		bool nodeExpanded = ImGui::TreeNodeEx((node->getName() + "##" + std::to_string(*count)).c_str(), flags);
		ImGui::PopStyleColor();

		// Set the inspected node if necessary: If clicking the non-arrow part of the tree-node, and once the
		// click is complete, and if the tree-node's corresponding node differs from the inspected node.
		if (
			!ImGui::IsItemToggledOpen() &&
			ImGui::IsItemFocused() &&
			ImGui::IsMouseReleased(ImGuiMouseButton_Left)
		)
		{
			EventManager::invokeEvent("Hierarchy.NodeSelected", node);
		}

		// Draw the right-click options, if user is right-clicking and hovering. If node gets deleted here, return.
		if (!drawNodeContextMenu(node)) goto cleanup;
		handleDragDrop(node);

		// If node was clicked on in the tree, display its children (and further descendants if their parent has previously been expanded).
		if (nodeExpanded)
		{
			// Nodes can be deleted during iteration, so cannot use for-each or iterator syntax.
			for (int i = 0; i < node->getChildCount(); i++)
			{
				drawTreeNode(node->getChild(i), &(++(*count)));
			}
		}

	cleanup:
		if (nodeExpanded)
		{
			// Need to pop tree even if node was deleted.
			ImGui::TreePop();
		}
	}


	void Hierarchy_::drawSiblingSeparator(Node *node)
	{
		ImGui::InvisibleButton("##HIERARCHY_SEPARATOR", ImVec2(100, 5));
		if (ImGui::BeginDragDropTarget())
		{
			bool unstable = false;

			// If the node is hovering over this button, move it after `node`.
			if (const ImGuiPayload *p = ImGui::AcceptDragDropPayload("HIERARCHY_NODE"))
			{
				Node *incoming = *(Node **)p->Data;
				size_t nodeIndex = node->getSiblingIndex();
				Node *parent = node->getParent();

				TE_INFO(std::format("Hierarchy: {} moves to sibling index {}, and parent is now", incoming->getName(), nodeIndex, parent->getName()));
				// Try and update the parent. If parent has not changed, setParent will return false.
				// If setParent returns false, we just need to set the sibling index as parent should already be `parent`.
				if (!incoming->setParent(parent, nodeIndex))
				{
					size_t incomingIndex = incoming->getSiblingIndex();

					// If we are increasing `incoming`'s index, we need to subtract 1 from the index supplied.
					// Moving `incomingIndex` to `nodeIndex` would put it *ahead* of `node` (only in this case).
					if (incomingIndex < nodeIndex)
						incoming->setSiblingIndex(nodeIndex - 1);
					else
						incoming->setSiblingIndex(nodeIndex);
				}
				unstable = true;
			}
			ImGui::EndDragDropTarget();

			if (unstable) return;
		}
	}


	void Hierarchy_::handleDragDrop(Node *node)
	{
		// Setup the Node payload created when dragging `node`.
		if (ImGui::BeginDragDropSource())
		{
			TE_INFO(std::format("{} source", node->getName()).c_str());

			ImGui::SetDragDropPayload("HIERARCHY_NODE", &node, sizeof(Node *));
			ImGui::EndDragDropSource();
		}

		// Setup what happens when a Node payload gets dropped on `node`.
		if (ImGui::BeginDragDropTarget())
		{
			// If the node was dropped onto `node`.
			if (const ImGuiPayload *p = ImGui::AcceptDragDropPayload("HIERARCHY_NODE"))
			{
				Node *incoming = *(Node **)p->Data;
				TE_INFO(std::format("Hierarchy: {} adopts {}", node->getName(), incoming->getName()));
				// Try and update the parent. If parent has not changed, setParent will return false.
				// If setParent returns false, `incoming` is already a child of `node`.
				// In that case, just move `incoming` to the end of `node`'s m_children.
				TE_INFO(std::format("{}", node->getChildCount()));
				if (!incoming->setParent(node)) incoming->setSiblingIndex(node->getChildCount() - 1);
			}
			ImGui::EndDragDropTarget();
		}
	}


	bool Hierarchy_::drawNodeContextMenu(Node *node)
	{
		bool nodeSurvives = true;

		// If item (node) is hovered and right-clicked...
		if (ImGui::BeginPopupContextItem())
		{
			Scene *activeScene = Scene::getActiveScene();
			bool isEditorControlled = node->IsEditorControlled();

			// If the node is not the root, not the current scene, and not an editor node, allow deletion
			bool deletionAllowed = node != m_currentRoot && node != activeScene && !isEditorControlled;
			if (deletionAllowed && ImGui::MenuItem("Delete Node"))
			{
				activeScene->onNodeDeleted(node);

				// Handle graceful degradation before node removal.
				EventManager::invokeEvent("Hierarchy.NodeDeleted", node);

				// Detach child from its parent.
				node->destroy();
				nodeSurvives = false;
			}

			// If the node is the root, allow toggling of editor node display
			bool toggleHierarchyAllowed = node == m_currentRoot;
			if (toggleHierarchyAllowed && ImGui::MenuItem("Toggle Editor Hierarchy"))
			{
				m_showEditorHierarchy = !m_showEditorHierarchy;
			}

			// If the node is not an editor node, allow child creation
			bool addChildAllowed = !isEditorControlled;
			if (addChildAllowed && ImGui::BeginMenu("Add Child Node"))
			{
				if (ImGui::MenuItem("Node")) addNewNode(node, new Node());
				if (ImGui::MenuItem("Text")) addNewNode(node, new Text());
				if (ImGui::MenuItem("Sprite (2D)")) addNewNode(node, new Sprite());
				if (ImGui::MenuItem("Model (3D)")) addNewNode(node, new Model());
				if (ImGui::MenuItem("Skybox (CubeMap)")) addNewNode(node, new CubeMap());
				if (ImGui::MenuItem("Point Light"))
				{
					PointLight *pl = new PointLight();
					auto gizmo = std::make_unique<Sprite>("Gizmo", Res("textures/gizmo/dir_light.png", true));
					pl->addChild(std::move(gizmo));
					addNewNode(node, pl);
				}
				if (ImGui::MenuItem("Directional Light"))
				{
					DirLight *dl = new DirLight();
					auto gizmo = std::make_unique<Sprite>("Gizmo", Res("textures/gizmo/dir_light.png", true));
					dl->addChild(std::move(gizmo));
					addNewNode(node, dl);
				}
				if (ImGui::MenuItem("Camera"))
				{
					Camera *cam = new Camera();
					auto gizmo = std::make_unique<Sprite>("Gizmo", Res("textures/gizmo/camera.png", true));
					cam->addChild(std::move(gizmo));
					addNewNode(node, cam);
				}
				if (ImGui::BeginMenu("Collider"))
				{
					if (ImGui::MenuItem("Sphere")) addNewNode(node, new Collider(
						"Collider: Sphere",
						std::make_unique<CollisionSphere>(node->getTransform(), 3, false)
					));
					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}
			
			ImGui::EndPopup();
		}

		return nodeSurvives;
	}


	void Hierarchy_::addNewNode(Node *parent, Node *heapAllocatedNode) const
	{
		parent->addChild(std::unique_ptr<Node>(heapAllocatedNode));
	}
}