#pragma once
#include "shapes/vertex.hpp"



namespace Tank
{
	class Texture;
	class IMeshContainer;
	class Shader;
	class Mesh
	{
		friend class IMeshContainer;
	private:
		unsigned int m_vao, m_vbo, m_ebo;
		std::vector<Vertex> m_vertices;
		std::vector<unsigned> m_indices;
		/// <summary>
		/// Shared only between meshes. E.g. multiple meshes can share the same texture.
		/// The same mesh may use a texture multiple times as well.
		/// </summary>
		std::vector<std::shared_ptr<Texture>> m_textures;

	public:
		Mesh(
			const std::vector<Vertex> &vertices,
			const std::vector<unsigned> &indices,
			const std::vector<std::shared_ptr<Texture>> &textures
		);
		virtual ~Mesh();


		const std::vector<Vertex> &getVertices() const { return m_vertices; }
		const std::vector<unsigned> &getIndices() const { return m_indices; }


		void draw(Shader *shader) const;
	};
}