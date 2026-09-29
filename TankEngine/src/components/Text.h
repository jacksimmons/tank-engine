#pragma once
#include <imgui.h>
#include <serialisation/Serialisation.h>


namespace Tank
{
	struct TextComponent
	{
		void draw();
	};


	template <>
	json serialise<TextComponent>(TextComponent *);
	template <>
	void deserialise<TextComponent>(const json &, TextComponent *);
}
