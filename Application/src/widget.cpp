#include <imgui.h>
#include "widget.hpp"


namespace Tank
{
	void Widget::textInput(const char *label, const std::string &hint, std::function<void(const std::string&)> onModified)
	{
		const size_t size = 100;
		char buf[size] = "\0";
		if (ImGui::InputTextWithHint(label, hint.c_str(), buf, size))
		{
			onModified(std::string(buf));
		}
	}


	void Widget::vec3Input(const char *label, glm::vec3 current, std::function<void(const glm::vec3&)> onModified)
	{
		float currentArr[] = { current.x, current.y, current.z };
		if (ImGui::InputFloat3(label, currentArr))
		{
			glm::vec3 modified = { currentArr[0], currentArr[1], currentArr[2] };
			onModified(modified);
		}
	}
}