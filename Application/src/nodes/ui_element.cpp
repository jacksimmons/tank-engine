#include <imgui.h>
#include "nodes/ui_element.hpp"


namespace Tank
{
	UIElement::UIElement(std::string name) : Node(name) {}


	void UIElement::draw() const
	{
		Node::draw();
	}
}