#pragma once
#include "window.hpp"


namespace Tank::Editor
{
	class _Console final : public _Window
	{
		friend class EditorApp;
	private:
		struct ConsoleCommand
		{
			std::string name;
			std::function<void()> callback;
		};

		std::vector<std::function<void()>> m_lines;
		std::vector<ConsoleCommand> m_commands;

		_Console(const std::string &name = "Console");
	public:
		virtual void drawPanel() override;

		void addLine(std::function<void()> line);
		void addColouredTextLine(ImVec4 colour, std::string text);
	};
}