#pragma once
#include "Schema.hpp"


namespace Tank::Editor::Schema
{
	void draw(bool start, const std::string &name, OnModified<bool> onModified);
}