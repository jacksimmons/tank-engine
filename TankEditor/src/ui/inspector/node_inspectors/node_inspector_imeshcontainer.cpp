#include <imgui.h>
#include <nodes/interfaces/mesh_container.hpp>
#include <shapes/vertex.hpp>
#include "node_inspector.hpp"


namespace Tank::Editor
{
	template <>
	void _NodeInspector<IMeshContainer>::draw()
	{
		if (ImGui::CollapsingHeader("Show Meshes##Inspector_Meshes"))
		{
			// Togglable info about all meshes
			auto meshes = m_node->getMeshes();
			for (int i = 0; i < meshes.size(); i++)
			{
				// Togglable info about this mesh
				auto &vertices = meshes[i]->getVertices();
				if (ImGui::CollapsingHeader(std::string{ "Mesh " + std::to_string(i) + " (" + std::to_string(vertices.size()) + " vertices)##Inspector_Mesh" }.c_str()))
				{
					// Display info about this mesh
					auto &indices = meshes[i]->getIndices();

					for (unsigned index : indices)
					{
						Vertex vertexThisIndex = vertices[index];
						ImGui::Text("Position: "); ImGui::SameLine();
						ImGui::Text(glm::to_string(vertexThisIndex.position).c_str()); ImGui::SameLine();
						ImGui::Text("Normal: "); ImGui::SameLine();
						ImGui::Text(glm::to_string(vertexThisIndex.normal).c_str()); ImGui::SameLine();
						ImGui::Text("TexCoords: "); ImGui::SameLine();
						ImGui::Text(glm::to_string(vertexThisIndex.texCoords).c_str());
					}
				}
			}
		}
	}
}