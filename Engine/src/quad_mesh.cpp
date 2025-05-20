#include "quad_mesh.hpp"


namespace Tank
{
	std::vector<Vertex> QuadMesh::s_vertices =
	{
		{.position = { -0.5f,  0.5f,  0.0f }, .normal = { 0.0f, 0.0f, -1.0f }, .texCoords = { 0.0f, 1.0f } },
		{.position = {  0.5f, -0.5f,  0.0f }, .normal = { 0.0f, 0.0f, -1.0f }, .texCoords = { 1.0f, 0.0f } },
		{.position = { -0.5f, -0.5f,  0.0f }, .normal = { 0.0f, 0.0f, -1.0f }, .texCoords = { 0.0f, 0.0f } },
		{.position = { -0.5f,  0.5f,  0.0f }, .normal = { 0.0f, 0.0f, -1.0f }, .texCoords = { 0.0f, 1.0f } },
		{.position = {  0.5f,  0.5f,  0.0f }, .normal = { 0.0f, 0.0f, -1.0f }, .texCoords = { 1.0f, 1.0f } },
		{.position = {  0.5f, -0.5f,  0.0f }, .normal = { 0.0f, 0.0f, -1.0f }, .texCoords = { 1.0f, 0.0f } },
	};

	std::vector<unsigned> QuadMesh::s_indices =
	{
		// Front face (CCW)
		3, 2, 1, 4, 0, 5,

		// Back face (CW)
		0, 1, 2, 3, 4, 5
	};
}