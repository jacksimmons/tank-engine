#include <imgui.h>
#include "console.hpp"
#include "widget.hpp"
#include "colours.hpp"


namespace Tank::Editor
{
	_Console::_Console(const std::string &name) : _Window(name, ImGuiWindowFlags_None, true)
	{
		m_commands.push_back({
			.name = "help",
			.callback =
			[this]() { 
				addLine([]() {
					ImGui::TextColored(
						Colour::INFO,
						"--- Global Keybinds ---\n"
						"ESC - Quit\n"
						"--- SceneView Keybinds ---\n"
						"F1 - Cycle glPolygonMode [=FILL], LINE, POINT\n"
						"F2 - Cycle glCullFace [=BACK], FRONT, FRONT_AND_BACK\n"
						"F3 - Cycle glFrontFace (winding order) [=CCW], CW\n"
						"F4 - Cycle glDepthFunc comparison [=LESS], NEVER, ALWAYS, LEQUAL, EQUAL, GREATER, GEQUAL, NOTEQUAL\n"
						"(x)AD (y)WS (z)QE - Camera pan, axis in brackets\n"
						"(x)JL (y)IK (z)UO - Camera rotation, axis in brackets\n"
					);
				});
			}
		});
	}


	void _Console::drawPanel()
	{
		std::string linesAsStr;
		for (auto line : m_lines)
		{
			line();
		}

		// Command input
		Widget::textInput("Enter a command...", "help", [this](const std::string &modified)
		{
			if (!ImGui::IsItemDeactivatedAfterEdit()) return;

			// For each valid console command...
			for (const auto &command : m_commands)
			{
				// If user input equals this command
				if (command.name == modified)
				{
					command.callback();
				}
			}
		});
	}


	void _Console::addLine(std::function<void()> line)
	{
		m_lines.push_back(line);

		// Maximum length of 100
		if (m_lines.size() >= 100) m_lines.erase(m_lines.begin());
	}


	void _Console::addColouredTextLine(ImVec4 colour, std::string text)
	{
		addLine(
			[colour, text]()
			{
				ImGui::TextColored(colour, text.c_str());
			}
		);
	}
}