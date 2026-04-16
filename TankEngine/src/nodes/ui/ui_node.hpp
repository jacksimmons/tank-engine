#pragma once
#include <nodes/node.hpp>


namespace Tank
{
	class UINode : public Node
	{
	public:
		UINode(const std::string &name) : Node(name) {}

		virtual void drawUI() = 0;
	};
}