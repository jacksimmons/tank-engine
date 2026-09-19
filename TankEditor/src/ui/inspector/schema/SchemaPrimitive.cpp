#include <imgui/imgui.h>
#include "SchemaPrimitive.h"


namespace Tank::Editor::Schema
{
	void draw(bool start, const std::string &name, OnModified<bool> onModified)
	{
		ImGui::Text(name.c_str());
		if (ImGui::Checkbox(("##BOOL_" + name).c_str(), &start))
			onModified(start);
	}
}