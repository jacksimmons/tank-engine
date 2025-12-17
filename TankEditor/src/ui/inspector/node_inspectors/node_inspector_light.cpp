#include <imgui.h>
#include <colours.hpp>
#include <widget.hpp>
#include <nodes/light.hpp>
#include "node_inspector.hpp"


namespace Tank::Editor
{
	/// <summary>
	/// Draws inspector section that is present for all Lights.
	/// Also handles drawing of all Light subclasses.
	/// </summary>
	template <>
	void _NodeInspector<Light>::draw()
	{
		ImGui::TextColored(Colour::TITLE, "Light Struct");
		std::string lightStruct = m_node->getLightStruct();
		ImGui::Text(lightStruct.c_str());

		ImGui::TextColored(Colour::TITLE, "Ambient Intensity (RGB)");
		Widget::vec3Input(
			"##Inspector_Light_Ambient",
			m_node->getAmbient(),
			[this](glm::vec3 newAmbient)
			{
				m_node->setAmbient(newAmbient);
			}
		);

		ImGui::TextColored(Colour::TITLE, "Diffuse Intensity (RGB)");
		Widget::vec3Input(
			"##Inspector_Light_Diffuse",
			m_node->getDiffuse(),
			[this](glm::vec3 newDiffuse)
			{
				m_node->setDiffuse(newDiffuse);
			}
		);

		ImGui::TextColored(Colour::TITLE, "Specular Intensity (RGB)");
		Widget::vec3Input(
			"##Inspector_Light_Specular",
			m_node->getSpecular(),
			[this](glm::vec3 newSpecular)
			{
				m_node->setSpecular(newSpecular);
			}
		);

		// Draw Light subclass sections
		if (DirLight *dirLight = dynamic_cast<DirLight *>(m_node))
		{
			ImGui::TextColored(Colour::TITLE, "Light Direction");
			Widget::vec3Input(
				"##Inspector_DirLight_Direction",
				dirLight->getDirection(),
				[this, &dirLight](glm::vec3 newDirection)
				{
					dirLight->setDirection(newDirection);
				}
			);
		}
	}
}