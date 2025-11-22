
#include <typeinfo>
#include <imgui.h>
#include "log.hpp"
#include "colours.hpp"
#include "nodes/camera.hpp"
#include "nodes/light.hpp"
#include "nodes/scene.hpp"
#include "nodes/cube_map.hpp"
#include "nodes/model.hpp"
#include "nodes/sprite.hpp"
#include "nodes/ui/text.hpp"
#include "ui/console.hpp"
#include "ui/hierarchy.hpp"
#include "ui/inspector/inspector.hpp"
#include "nodes/physics/collider.hpp"
#include "shapes/collision_sphere.hpp"


namespace Tank::Editor
{
	const WindowOpts WINDOW_OPTS = {
		ImGuiWindowFlags_None,
		true,
		false
	};


	_Hierarchy::_Hierarchy(const std::string &name)
		: _Window(name, WINDOW_OPTS)
	{
		m_showEditorHierarchy = false;
		m_currentRoot = Tank::Scene::getActiveScene();
	}


	void _Hierarchy::drawPanel()
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
	void _Hierarchy::drawTreeNode(Node *node, int *count)
	{
		// Base case.
		if (!node) return;

		_Inspector *inspector = (_Inspector*)getSibling("Inspector");
		size_t childCount = node->getChildCount();

		// Determine if leaf node.
		ImGuiTreeNodeFlags flags = 0;
		if (childCount == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;

		// Add node name to the tree, clicking on this node will set `nodeExpanded` to true if not a leaf node.
		ImVec4 nodeNameCol = Colour::NORMAL;
		if (!node->getEnabled()) nodeNameCol = Colour::DISABLED;
		if (node->isEditorControlled()) nodeNameCol = Colour::ERR;
		
		ImGui::PushStyleColor(ImGuiCol_Text, nodeNameCol);
		bool nodeExpanded = ImGui::TreeNodeEx((node->getName() + "##" + std::to_string(*count)).c_str(), flags);
		ImGui::PopStyleColor();
		
		// Set the inspected node if necessary: If clicking the non-arrow part of the tree-node, and once the
		// click is complete, and if the tree-node's corresponding node differs from the inspected node.
		if (
			!ImGui::IsItemToggledOpen() &&
			ImGui::IsItemFocused() &&
			!ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
			inspector &&
			inspector->getInspectedNode() != node)
		{
			inspector->setInspectedNode(node);
		}

		// Draw the right-click options, if user is right-clicking and hovering. If node gets deleted here, return.
		if (!drawNodeContextMenu(node, inspector)) goto cleanup;

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


	bool _Hierarchy::drawNodeContextMenu(Node *node, _Inspector *inspector)
	{
		bool nodeSurvives = true;

		// If item (node) is hovered and right-clicked...
		if (ImGui::BeginPopupContextItem())
		{
			Scene *activeScene = Scene::getActiveScene();
			bool isEditorControlled = node->isEditorControlled();
			
			// If the node is not the root, not the current scene, and not an editor node, allow deletion
			if (node != m_currentRoot && node != activeScene && !isEditorControlled && ImGui::MenuItem("Delete Node"))
			{
				activeScene->onNodeDeleted(node);

				// Handle graceful degradation before node removal.
				inspector->onNodeDeleted(node);

				// Detach child from its parent.
				node->destroy();
				nodeSurvives = false;
			}

			// If the node is the root, allow toggling of editor node display
			if (node == m_currentRoot && ImGui::MenuItem("Toggle Editor Hierarchy"))
			{
				m_showEditorHierarchy = !m_showEditorHierarchy;
			}

			// If the node is not an editor node, allow child creation
			if (!isEditorControlled && ImGui::BeginMenu("Add Child Node"))
			{
				if (ImGui::MenuItem("Node")) addNewNode(node, new Node());
				if (ImGui::MenuItem("Text")) addNewNode(node, new Text());
				if (ImGui::MenuItem("Sprite (2D)"))
				{
					ShaderSources sources;
					sources.vertex.location = "shader.vert";
					sources.fragment.location = "shader.frag";
					addNewNode(node, new Sprite("Sprite", sources, "textures/awesomeface.png"));
				}
				if (ImGui::MenuItem("Model (3D)"))
				{
					ShaderSources sources;
					sources.vertex.location = "shader.vert";
					sources.fragment.location = "shader.frag";
					addNewNode(node, new Model("Model", sources, "backpack/backpack.obj"));
				}
				if (ImGui::MenuItem("Skybox (CubeMap)"))
				{
					ShaderSources sources;
					sources.vertex.location = "skybox.vert";
					sources.fragment.location = "skybox.frag";
					addNewNode(node, new CubeMap("CubeMap", sources));
				}
				if (ImGui::MenuItem("Point Light")) addNewNode(node, new PointLight());
				if (ImGui::MenuItem("Directional Light")) addNewNode(node, new DirLight());
				if (ImGui::MenuItem("Camera")) addNewNode(node, new Camera());
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


	void _Hierarchy::addNewNode(Node *parent, Node *heapAllocatedNode) const
	{
		parent->addChild(std::unique_ptr<Node>(heapAllocatedNode));
	}
}