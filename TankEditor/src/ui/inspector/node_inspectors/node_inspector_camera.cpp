#include <imgui.h>
#include <glm/gtx/string_cast.hpp>
#include <widget.hpp>
#include <colours.hpp>
#include <nodes/scene.hpp>
#include <nodes/camera.hpp>
#include "node_inspector.hpp"


namespace Tank::Editor
{
	/// <summary>
	/// Draws inspector section that is present for all Cameras.
	/// </summary>
	template <>
	void _NodeInspector<Camera>::draw()
	{
		ImGui::TextColored(Colour::TITLE, "Camera Eye");
		glm::vec3 eye = m_node->getTransformedEye();
		ImGui::Text(glm::to_string(eye).c_str());

		ImGui::TextColored(Colour::TITLE, "Camera Centre");
		glm::vec3 centre = m_node->getTransformedCentre();
		ImGui::Text(glm::to_string(centre).c_str());

		ImGui::TextColored(Colour::TITLE, "Camera Up");
		glm::vec3 up = m_node->getTransformedUp();
		ImGui::Text(glm::to_string(up).c_str());

		ImGui::TextColored(Colour::TITLE, "Camera Pan Speed");
		Widget::floatInput("##Inspector_Camera_PanSpd", m_node->getPanSpeed(), [this](float modified) { m_node->setPanSpeed(modified); });

		ImGui::TextColored(Colour::TITLE, "Camera Rotation Speed");
		Widget::floatInput("##Inspector_Camera_RotSpd", m_node->getRotSpeed(), [this](float modified) { m_node->setRotSpeed(modified); });

		ImGui::TextColored(Colour::TITLE, "Culling Distance");
		ImGui::Text("Near"); ImGui::SameLine();
		Widget::floatInput("##Inspector_Camera_CullNear", m_node->getCullNear(), [this](float modified) { m_node->setCullNear(modified); });
		ImGui::Text("Far"); ImGui::SameLine();
		Widget::floatInput("##Inspector_Camera_CullFar", m_node->getCullFar(), [this](float modified) { m_node->setCullFar(modified); });
	}
}