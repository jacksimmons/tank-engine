#pragma once
#include "shapes/vertex.hpp"


namespace Tank
{
	class TANK_API CollisionShape
	{
	private:
		std::vector<Face> m_faces;
	public:
		CollisionShape(const std::vector<Vertex> &vertices, const std::vector<unsigned> &indices, unsigned faceVertexCount = 3);
		virtual ~CollisionShape() = default;
	};
}