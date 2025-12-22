#pragma once
#include "schema.hpp"


namespace Tank::Editor::Schema
{
	void draw(const glm::vec3 &start, const std::string &name, OnModified<glm::vec3> onModified);
	void draw(const glm::mat4 &start, const std::string &name);
}
