#include <imgui.h>
#include <widget.hpp>
#include <colours.hpp>
#include <nodes/node.hpp>
#include <nodes/camera.hpp>
#include <nodes/scene.hpp>
#include <scripting/script.hpp>
#include "node_inspector.hpp"


namespace Tank::Editor
{
	/// <summary>
	/// Draws inspector section that is present for all Nodes.
	/// </summary>
	template <>
	void _NodeInspector<Node>::draw()
	{
		Tank::Transform *transform = m_node->getTransform();
		const glm::mat4 &modelMatrix = transform->getWorldModelMatrix();

		bool enabled = m_node->Enabled();
		if (ImGui::Checkbox("Enabled", &enabled))
			m_node->Enabled = enabled;

		bool visible = m_node->Visible();
		if (ImGui::Checkbox("Visible", &visible))
			m_node->Visible = visible;

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

		ImGui::TextColored(Colour::TITLE, "Scripts");
		std::vector<Res> scriptPaths = m_node->getScriptPaths();
		if (scriptPaths.empty())
		{
			ImGui::TextColored(Colour::DISABLED, "None");
		}

		for (const Res &path : scriptPaths)
		{
			ImGui::Text(Res::encode(path).c_str());
			//Widget::textInput(
			//	std::format("##Inspector_Script_{}", i).c_str(),
			//	path.string(),
			//	[this](std::string newPath)
			//	{
			//		auto script = Script::createScript(m_node, newPath);
			//		if (script.has_value())
			//			m_node->addScript(std::move(script.value()));
			//	}
			//);
		}
		// Allow user to add a new script
		Widget::textInput(
			"##Inspector_Add_Script",
			"Add Script",
			[this, &scriptPaths](std::string newPath)
			{
				auto script = Script::createScript(m_node, Res::decode(newPath));
				for (auto it = scriptPaths.begin(); it != scriptPaths.end(); ++it)
				{
					if (!script.has_value()) break;

					if ((*it) == Res::decode(newPath))
					{
						m_node->addScript(std::move(script.value()));
					}
				}
			}
		);
	}
}