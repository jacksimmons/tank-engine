#pragma once
#include "mesh.hpp"


namespace Tank
{
	class TANK_API QuadMesh : public Mesh
	{
	private:
		static std::vector<Vertex> s_vertices;
		static std::vector<unsigned> s_indices;
	public:
		QuadMesh(const std::vector<std::shared_ptr<Texture>> &textures)
			: Mesh(s_vertices, s_indices, textures) {}
		virtual ~QuadMesh() = default;
	};
}