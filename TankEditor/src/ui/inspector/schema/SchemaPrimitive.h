#pragma once
#include "Schema.h"


namespace Tank::Editor::Schema
{
	void draw(bool start, const std::string &name, OnModified<bool> onModified);
}