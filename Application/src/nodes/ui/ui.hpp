#pragma once

#include "nodes/node.hpp"


namespace Tank
{
	class UI : public Node
	{
	public:
		UI(const std::string &name);
		virtual ~UI() = default;

		void draw() override;
		virtual void drawUI() = 0;
	};
}