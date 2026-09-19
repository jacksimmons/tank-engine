#include <Log.h>
#include "Colours.h"
#include "NodeInspectors/NodeInspector.h"
#include "nodes/Camera.h"
#include "nodes/interfaces/ShaderContainer.h"
#include "nodes/Light.h"
#include "nodes/Model.h"
#include "nodes/Node.h"
#include "nodes/Scene.h"
#include "nodes/Sprite.h"
#include "nodes/Audio.h"
#include "nodes/ui/Text.h"
#include "ui/FileDialog.h"
#include "ui/inspector/Inspector.h"
#include <events/EventManager.h>
#include <imgui.h>
#include <nodes/interfaces/MeshContainer.h>


namespace Tank::Editor
{
	const WindowOpts WINDOW_OPTS = {
		ImGuiWindowFlags_None,
		false,
		false
	};


	Inspector_::Inspector_(const std::string &name)
		: _Window(name, WINDOW_OPTS)
	{
		m_inspectedNode = nullptr;

		/// <summary>
		/// Handle removing outline shader for the currently selected node, and applying
		/// outline shader to the new selected node.
		/// Then update the inspected node variable.
		/// </summary>
		auto onNodeSelected = EventManager::getEvent<Node*>("Hierarchy.NodeSelected");
		onNodeSelected->registerHandler([this](Node *node)
		{
			// Don't allow editor nodes in the hierarchy to be inspected
			if (node->IsEditorControlled())
			{
				TE_WARN("Attempted to inspect an editor node - this is unsupported as it's extremely unstable.");
				return;
			}

			if (m_inspectedNode)
			{
				// Disable outline for current inspected node (if necessary)
				// @todo Access violation after New Project (inspector stays open)
				if (IOutlined *inspectedOutline = dynamic_cast<IOutlined*>(m_inspectedNode))
				{
					inspectedOutline->setOutlineEnabled(false);
				}
			}

			// Enable outline for new inspected node (if necessary)
			if (IOutlined *ioutlined = dynamic_cast<IOutlined*>(node))
			{
				ioutlined->setOutlineEnabled(true);
			}

			// Set the inspected node, before trying to setup node inspectors
			m_inspectedNode = node;

			// Now setup a node inspector, for each type which has one.
			m_nodeInspectors.clear(); // Clear beforehand
			tryAddSection<Node>();
			tryAddSection<_Scene>();
			tryAddSection<Camera>();
			tryAddSection<Light>();
			tryAddSection<Audio>();
			tryAddSection<Sprite>();
			tryAddSection<Model>();
			tryAddSection<IMeshContainer>();
			tryAddSection<IShaderContainer>();
		});

		/// <summary>
		/// Recurse over all descendants of node, and if any match to the inspected
		/// node, set the inspected node to nullptr (to reflect the deletion).
		/// </summary>
		auto onNodeDeleted = EventManager::getEvent<Node*>("Hierarchy.NodeDeleted");
		onNodeDeleted->registerHandler([this](Node *node)
		{
			node->forEachDescendant(
				[this](Node *node)
				{
					if (node == m_inspectedNode)
						m_inspectedNode = nullptr;
				},
				[this]()
				{
					return m_inspectedNode == nullptr;
				}
			);
		});
	}


	void Inspector_::drawPanel()
	{
		if (m_inspectedNode)
		{
			ImGui::TextColored(Tank::Colour::TITLE, "Type");
			ImGui::Text(typeid(*m_inspectedNode).name());
			
			// Draw all node inspector sections
			for (const auto &nodeInspector : m_nodeInspectors)
			{
				nodeInspector->draw();
			}
		}
		else
		{
			m_nodeInspectors.clear();
		}
	}


	template <class T>
	void Inspector_::tryAddSection()
	{
		// If the node can be casted to this node subtype, then draw using the subtype inspector.
		if (T *t = dynamic_cast<T *>(m_inspectedNode))
		{
			m_nodeInspectors.push_back(std::make_unique<NodeInspector_<T>>(t, this));
		}
	}
}
