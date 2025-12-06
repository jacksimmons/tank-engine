#include <imgui.h>
#include <events/event_manager.hpp>
#include "console.hpp"
#include "widget.hpp"
#include "colours.hpp"


namespace Tank::Editor
{
	const WindowOpts WINDOW_OPTS = {
		ImGuiWindowFlags_None,
		true,
		true
	};


	_Console::_Console(const std::string &name) : _Window(name, WINDOW_OPTS)
	{
		// Register event handlers
		EventManager::getEvent<std::string, ImColor>("Console.AddColouredLine")
		->registerHandler(
			[this](const std::string &line, const ImColor &colour)
			{
				this->addColouredTextLine(colour, line);
			}
		);
	}


	void _Console::drawPanel()
	{
		std::string linesAsStr;
		for (auto line : m_lines)
		{
			line();
		}

		// Command input
		Widget::textInput("##CONSOLE_ENTER_COMMAND", "help", [this](const std::string &modified)
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

		// Control buttons
		if (ImGui::Button("Clear##CONSOLE_CLEAR")) m_lines.clear();
		ImGui::SameLine();
		if (ImGui::Button("Help##CONSOLE_HELP"))
		{
			addLine([]()
			{
				ImGui::TextColored(
					Colour::INFO,
					"--- Global Keybinds ---\n"
					"--- SceneView Keybinds ---\n"
					"F1 - Cycle glPolygonMode [=FILL], LINE, POINT\n"
					"F2 - Cycle glCullFace [=BACK], FRONT, FRONT_AND_BACK\n"
					"F3 - Cycle glFrontFace (winding order) [=CCW], CW\n"
					"F4 - Cycle glDepthFunc comparison [=LESS], NEVER, ALWAYS, LEQUAL, EQUAL, GREATER, GEQUAL, NOTEQUAL\n"
					"F5 - Cycle glBlendFunc sfactor ZERO, ONE, ...\n"
					"F6 - Cycle glBlendFunc dfactor ZERO, ONE, ...\n"
					"(x)AD (y)WS (z)QE - Camera pan, axis in brackets\n"
					"(x)JL (y)IK (z)UO - Camera rotation, axis in brackets\n"
				);
			});
		}
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