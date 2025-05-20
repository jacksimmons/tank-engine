#include "collision_shape.hpp"


namespace Tank
{
	CollisionShape::CollisionShape(const std::vector<Vertex> &vertices, const std::vector<unsigned> &indices, unsigned faceVertexCount)
	{
		std::vector<Face> faces;

		// Divide the object into 
		for (size_t i = 0; i < indices.size(); i += faceVertexCount)
		{
			Face face;
			for (size_t j = i; j < i + faceVertexCount; j++)
			{
				face.traversal.push_back(vertices[indices[j]]);
			}
		}
	}
}