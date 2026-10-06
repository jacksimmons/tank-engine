#include "Text.h"


// Flags for a dummy window
static const ImGuiWindowFlags g_dummyWindowFlags = ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoMove |
	ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoSavedSettings;


namespace Tank
{
	void TextComponent::draw()
	{
		ImGui::Begin("##Text", NULL, g_dummyWindowFlags);
		ImGui::Text("HI");
		ImGui::End();
	}


	// =======================
	//		Serialisation
	// =======================
	//template <>
	//json serialise<TextComponent>(TextComponent *)
	//{
	//	return {};
	//}

	//template <>
	//void deserialise<TextComponent>(const json &, TextComponent *)
	//{
	//}
}
