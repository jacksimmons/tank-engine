#include <imgui.h>
#include <widget.hpp>
#include <colours.hpp>
#include <nodes/node.hpp>
#include <nodes/camera.hpp>
#include <nodes/scene.hpp>
#include "node_inspector.hpp"


namespace Tank::Editor
{
	/// <summary>
	/// Draws inspector section that is present for all Nodes.
	/// </summary>
	void _NodeInspector<Node>::draw()
	{
		Tank::Transform *transform = m_node->getTransform();
		const glm::mat4 &modelMatrix = transform->getWorldModelMatrix();

		bool enabled = m_node->getEnabled();
		if (ImGui::Checkbox("Enabled", &enabled))
			m_node->setEnabled(enabled);

		bool visible = m_node->getVisibility();
		if (ImGui::Checkbox("Visible", &visible))
			m_node->setVisibility(visible);

		ImGui::TextColored(Tank::Colour::TITLE, "Name");
		Tank::Widget::textInput("##Inspector_Name", m_node->getName(),
			[this](const std::string &newName)
			{
				m_node->setName(newName);
			}
		);

		if (ImGui::Button("<Snap To>"))
		{
			auto cam = Tank::Scene::getActiveScene()->getActiveCamera();
			if (cam != nullptr)
			{
				glm::mat4 worldMatrix = transform->getWorldModelMatrix();
				cam->setPosition(Tank::mat4::getTranslation(worldMatrix));
				cam->setRotation(Tank::mat4::getRotation(worldMatrix));
			}
		}

		ImGui::TextColored(Tank::Colour::TITLE, "Model Matrix");
		// glm::mat4 indexing is column-major, but ImGui is row-major.
		// Transposing the model means an ImGui row corresponds to a model matrix row.
		glm::mat4 displayMatrix = glm::transpose(modelMatrix);
		for (int i = 0; i < 4; i++)
		{
			glm::vec4 row = displayMatrix[i];
			std::string rowText = std::to_string(row.x) + "\t" +
				std::to_string(row.y) + "\t" +
				std::to_string(row.z) + "\t" +
				std::to_string(row.w);
			ImGui::Text(rowText.c_str());
		}

		ImGui::TextColored(Colour::TITLE, "Translation");
		Widget::vec3Input(
			"##Inspector_Translation",
			transform->getLocalTranslation(),
			[&transform](glm::vec3 newTranslation)
			{
				transform->setLocalTranslation(newTranslation);
			}
		);

		ImGui::TextColored(Colour::TITLE, "Scale");
		Widget::vec3Input(
			"##Inspector_Scale",
			transform->getLocalScale(),
			[&transform](glm::vec3 newScale)
			{
				transform->setLocalScale(newScale);
			}
		);

		ImGui::TextColored(Colour::TITLE, "Rotation (Euler Angles)");
		Widget::vec3Input(
			"##Inspector_Rotation_EulerAngles",
			glm::eulerAngles(transform->getLocalRotation()),
			[&transform](glm::vec3 newRotation)
			{
				transform->setLocalRotation(newRotation);
			}
		);
	}
}