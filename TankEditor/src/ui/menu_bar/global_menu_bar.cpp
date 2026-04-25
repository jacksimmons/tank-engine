#include <glad/glad.h>
#include <imgui/imgui.h>
#include "global_menu_bar.hpp"


namespace Tank::Editor
{
	void _GlobalMenuBar::drawMainMenuBar()
	{
		if (ImGui::BeginMenu("System"))
		{
			drawSystem();
			ImGui::EndMenu();
		}
	}


	void _GlobalMenuBar::drawSystem()
	{
		// GPU manufacturer (e.g. NVIDIA Corporation)
		ImGui::Text(std::format("GPU manufacturer: {}", (const char *)glGetString(GL_VENDOR)).c_str());
		// GPU hardware name (e.g. NVIDIA GeForce RTX 5090)
		ImGui::Text(std::format("GPU device: {}", (const char *)glGetString(GL_RENDERER)).c_str());
		// OpenGL version & driver (e.g. 4.3.0 NVIDIA 591.86)
		ImGui::Text(std::format("OpenGL version: {}", (const char *)glGetString(GL_VERSION)).c_str());
	}
}