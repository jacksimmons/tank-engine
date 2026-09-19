#pragma once
#include <nodes/Node.h>


namespace Tank
{
	class UINode : public Node
	{
	public:
		UINode(const std::string &name) : Node(name) {}

		virtual void drawUI() = 0;
	};
}