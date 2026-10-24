#include <Log.h>
#include "Colours.h"
#include "NodeInspectors/NodeInspector.h"
#include "components/Camera.h"
#include "components/Light.h"
#include "components/Model.h"
#include "scene/Entity.h"
#include "components/Sprite.h"
#include "components/Audio.h"
#include "ui/FileDialog.h"
#include "ui/inspector/Inspector.h"
#include <events/EventManager.h>
#include <events/EditorEventID.h>
#include <imgui.h>
#include <components/interfaces/MeshContainer.h>


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
		m_selectedEntity = nullptr;

		/// <summary>
		/// Handle removing outline shader for the currently selected node, and applying
		/// outline shader to the new selected node.
		/// Then update the inspected node variable.
		/// </summary>
		auto onNodeSelected = EventManager::getEvent<GameEntity*>(TANK_EVENT_HIERARCHY_NODE_SELECTED);
		onNodeSelected->registerHandler([this](Node *node)
		{
			// Don't allow editor nodes in the hierarchy to be inspected
			if (node->isEditorControlled())
			{
				TE_WARN("Attempted to inspect an editor node - this is unsupported as it's extremely unstable.");
				return;
			}

			if (m_selectedEntity)
			{
				// Disable outline for current inspected node (if necessary)
				// @todo Access violation after New Project (inspector stays open)
				if (IOutlined *inspectedOutline = dynamic_cast<IOutlined*>(m_selectedEntity))
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
			m_selectedEntity = node;

			// Now setup a node inspector, for each type which has one.
			m_nodeInspectors.clear(); // Clear beforehand
			tryAddSection<Node>();
			tryAddSection<Scene>();
			tryAddSection<SceneCamera>();
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
					if (node == m_selectedEntity)
						m_selectedEntity = nullptr;
				},
				[this]()
				{
					return m_selectedEntity == nullptr;
				}
			);
		});
	}


	void Inspector_::drawPanel()
	{
		if (m_selectedEntity)
		{
			ImGui::TextColored(Tank::Colour::TITLE, "Type");
			ImGui::Text(typeid(*m_selectedEntity).name());
			
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
		if (T *t = dynamic_cast<T *>(m_selectedEntity))
		{
			m_nodeInspectors.push_back(std::make_unique<NodeInspector_<T>>(t, this));
		}
	}
}
