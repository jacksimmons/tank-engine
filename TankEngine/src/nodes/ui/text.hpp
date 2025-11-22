#pragma once
#include <imgui.h>
#include "../node.hpp"


namespace Tank
{
	class TANK_API Text : public Node
	{
	public:
		virtual void draw() override;
	};
}