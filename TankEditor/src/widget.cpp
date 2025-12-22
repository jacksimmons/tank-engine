#include <imgui.h>
#include "widget.hpp"
#include "log.hpp"


namespace Tank
{
	void Widget::textInput(const char *label, const std::string &hint, std::function<void(const std::string&)> onModified, const std::string &contents)
	{
		const size_t size = 100;
		char buf[size] = "\0";

		if (contents.length() > size)
		{
			TE_CORE_CRITICAL("'contents' was too large!");
			return;
		}

		strcpy(buf, contents.data());
		if (ImGui::InputTextWithHint(label, hint.c_str(), buf, size))
		{
			onModified(std::string(buf));
		}
	}


	void Widget::floatInput(const char *label, float current, std::function<void(float)> onModified)
	{
		if (ImGui::InputFloat(label, &current))
		{
			onModified(current);
		}
	}
}