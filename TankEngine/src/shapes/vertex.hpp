#pragma once


namespace Tank
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	struct Face
	{
		std::vector<Vertex> traversal;
	};
}