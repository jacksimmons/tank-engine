#include <imgui.h>
#include <widget.hpp>
#include <colours.hpp>
#include <nodes/node.hpp>
#include <nodes/camera.hpp>
#include <nodes/scene.hpp>
#include <scripting/script.hpp>
#include <ui/inspector/schema/schema_primitive.hpp>
#include <ui/inspector/schema/schema_glm.hpp>
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

		Schema::draw(m_node->Enabled(), "Enabled", [this](bool val) { m_node->Enabled = val; });
		Schema::draw(m_node->Visible(), "Visible", [this](bool val) { m_node->Visible = val; });

		ImGui::TextColored(Tank::Colour::TITLE, "Name");
		Tank::Widget::textInput("##Inspector_Name", m_node->getName(),
			[this](const std::string &newName)
			{
				if (newName != m_node->getName()) m_node->setName(newName);
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

		ImGui::TextColored(Tank::Colour::TITLE, "Transform");
		Schema::draw(modelMatrix, "Model Matrix");
		Schema::draw(transform->getLocalTranslation(), "Translation", [&transform](const glm::vec3 &val)
		{
			transform->setLocalTranslation(val);
		});
		Schema::draw(transform->getLocalScale(), "Scale", [&transform](const glm::vec3 &val)
		{
			transform->setLocalScale(val);
		});
		Schema::draw(glm::eulerAngles(transform->getLocalRotation()), "Rotation (Euler Angles)", [&transform](const glm::vec3 &val)
		{
			transform->setLocalRotation(val);
		});

		ImGui::TextColored(Colour::TITLE, "Scripts");
		std::vector<Res> scriptPaths = m_node->getScriptPaths();
		if (scriptPaths.empty())
		{
			ImGui::TextColored(Colour::DISABLED, "None");
		}

		for (const Res &path : scriptPaths)
		{
			ImGui::Text(Res::encode(path).c_str());
			ImGui::SameLine();
			if (ImGui::Button("Open in VSCode"))
			{
				system(std::format("code {}", path.resolvePathStr()).c_str());
			}

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