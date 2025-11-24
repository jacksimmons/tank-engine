#include <optional>
#include <imgui.h>
#include <glm/gtx/euler_angles.hpp>
#include "transform.hpp"
#include "colours.hpp"
#include "file.hpp"
#include "widget.hpp"
#include "shader.hpp"
#include "nodes/node.hpp"
#include "nodes/scene.hpp"
#include "nodes/light.hpp"
#include "nodes/camera.hpp"
#include "nodes/sprite.hpp"
#include "nodes/model.hpp"
#include "nodes/ui/text.hpp"
#include "ui/console.hpp"
#include "ui/file_dialog.hpp"
#include "ui/inspector/inspector.hpp"
#include "nodes/interfaces/editor_only.hpp"
#include "nodes/interfaces/shader_container.hpp"
#include "node_inspectors/node_inspector.hpp"


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

			// Draw a section for each subtype the node belongs to.
			tryDrawSection<Node>();
			tryDrawSection<Scene>();
			tryDrawSection<Camera>();
			tryDrawSection<Light>();
			tryDrawSection<Sprite>();
			tryDrawSection<Model>();
			tryDrawSection<IMeshContainer>();
			tryDrawSection<IShaderContainer>();
		}
	}


	template <class T>
	void _Inspector::tryDrawSection()
	{
		// If the node can be casted to this node subtype, then draw using the subtype inspector.
		if (T *t = dynamic_cast<T *>(m_inspectedNode))
		{
			m_nodeInspector.reset();
			m_nodeInspector = std::make_unique<_NodeInspector<T>>(t, this);
			m_nodeInspector->draw();
		}
	}


	/// <summary>
	/// Recurse over all descendants of node, and if any match to the inspected
	/// node, set the inspected node to nullptr (to reflect the deletion).
	/// </summary>
	void _Inspector::onNodeDeleted(Node *node)
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
	}


	/// <summary>
	/// Sets the inspected node for the inspector.
	/// </summary>
	void _Inspector::setInspectedNode(Node *node)
	{
		if (m_inspectedNode)
		{
			// Disable outline for prev inspected node (if necessary)
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

		m_inspectedNode = node;
	}
}