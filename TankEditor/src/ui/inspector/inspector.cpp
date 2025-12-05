#include "colours.hpp"
#include "file.hpp"
#include "node_inspectors/node_inspector.hpp"
#include "nodes/camera.hpp"
#include "nodes/interfaces/editor_only.hpp"
#include "nodes/interfaces/shader_container.hpp"
#include "nodes/light.hpp"
#include "nodes/model.hpp"
#include "nodes/node.hpp"
#include "nodes/scene.hpp"
#include "nodes/sprite.hpp"
#include "nodes/ui/text.hpp"
#include "shader.hpp"
#include "ui/console.hpp"
#include "ui/file_dialog.hpp"
#include "ui/hierarchy.hpp"
#include "ui/inspector/inspector.hpp"
#include "widget.hpp"
#include <events/event_manager.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <imgui.h>
#include <nodes/interfaces/mesh_container.hpp>
#include <optional>


namespace Tank::Editor
{
	const WindowOpts WINDOW_OPTS = {
		ImGuiWindowFlags_None,
		true,
		false
	};


	_Inspector::_Inspector(const std::string &name)
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
			if (m_inspectedNode)
			{
				// Disable outline for current inspected node (if necessary)
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
			tryAddSection<Scene>();
			tryAddSection<Camera>();
			tryAddSection<Light>();
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


	void _Inspector::drawPanel()
	{
		if (m_inspectedNode)
		{
			ImGui::TextColored(Tank::Colour::TITLE, "Type");
			ImGui::Text(typeid(*m_inspectedNode).name());

			// Prevent users from modifying editor nodes, which would likely cause a crash.
			if (m_inspectedNode->isEditorControlled())
			{
				ImGui::Text("Modifying the editor is not supported.");
				return;
			}

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
	void _Inspector::tryAddSection()
	{
		// If the node can be casted to this node subtype, then draw using the subtype inspector.
		if (T *t = dynamic_cast<T *>(m_inspectedNode))
		{
			m_nodeInspectors.push_back(std::make_unique<_NodeInspector<T>>(t, this));
		}
	}
}