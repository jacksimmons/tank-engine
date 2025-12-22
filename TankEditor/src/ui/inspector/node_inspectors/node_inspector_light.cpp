#include <imgui.h>
#include <colours.hpp>
#include <ui/inspector/schema/schema_glm.hpp>
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

		Schema::draw(m_node->getAmbient(), "Ambient Intensity (RGB)", [this](const glm::vec3 &val)
		{
			m_node->setAmbient(val);
		});
		Schema::draw(m_node->getDiffuse(), "Diffuse Intensity (RGB)", [this](const glm::vec3 &val)
		{
			m_node->setDiffuse(val);
		});
		Schema::draw(m_node->getSpecular(), "Specular Intensity (RGB)", [this](const glm::vec3 &val)
		{
			m_node->setSpecular(val);
		});

		// Draw Light subclass sections
		if (DirLight *dirLight = dynamic_cast<DirLight *>(m_node))
		{
			ImGui::TextColored(Colour::TITLE, "Light Direction");
			Schema::draw(dirLight->getDirection(), "Light Direction", [&dirLight](const glm::vec3 &val)
			{
				dirLight->setDirection(val);
			});
		}
	}
}