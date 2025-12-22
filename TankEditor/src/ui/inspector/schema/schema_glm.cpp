#include <imgui/imgui.h>
#include "schema_glm.hpp"


namespace Tank::Editor::Schema
{
	//
	// =-=-=-=-= Vectors =-=-=-=-=
	//
	void draw(const glm::vec3 &start, const std::string &name, OnModified<glm::vec3> onModified)
	{
		ImGui::Text(name.c_str());

		float currentArr[] = { start.x, start.y, start.z };
		if (ImGui::InputFloat3(("##VEC3_" + name).c_str(), currentArr))
		{
			onModified({ currentArr[0], currentArr[1], currentArr[2] });
		}
	}


	// =-=-=-=-= Matrices =-=-=-=-=
	void draw(const glm::mat4 &start, const std::string &name)
	{
		ImGui::Text(name.c_str());

		// glm::mat4 indexing is column-major, but ImGui is row-major.
		// Transposing the model means an ImGui row corresponds to a model matrix row.
		glm::mat4 displayMatrix = glm::transpose(start);
		for (int i = 0; i < 4; i++)
		{
			glm::vec4 row = displayMatrix[i];
			std::string rowText = std::to_string(row.x) + "\t" +
				std::to_string(row.y) + "\t" +
				std::to_string(row.z) + "\t" +
				std::to_string(row.w);
			ImGui::Text(rowText.c_str());
		}
	}
}