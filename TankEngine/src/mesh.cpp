#include <glad/glad.h>
#include "mesh.hpp"
#include "texture.hpp"
#include "shader.hpp"


namespace Tank
{
	Mesh::Mesh(
		const std::vector<Vertex> &vertices,
		const std::vector<unsigned int> &indices,
		const std::vector<std::shared_ptr<Texture>> &textures)
		: m_vertices(vertices), m_indices(indices), m_textures(textures)
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// Vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// Vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		// Vertex tex coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		glBindVertexArray(0);
	}


	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
	}


	void Mesh::draw(const Shader &shader) const
	{
		unsigned int diffuseIdx = 0;
		unsigned int specularIdx = 0;

		shader.use();
		
		for (unsigned int i = 0; i < m_textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::string name = m_textures[i]->getTexType();
			std::string number;

			if (name == "diffuse")
				number = std::to_string(diffuseIdx++);
			else if (name == "specular")
				number = std::to_string(specularIdx++);

			shader.setInt("material." + name + "[" + number + "]", i);

			glBindTexture(GL_TEXTURE_2D, m_textures[i]->getTexID());
		}

		// Draw mesh vertices
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		shader.unuse();
	}
}