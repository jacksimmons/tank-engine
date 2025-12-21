#pragma once
#include "nodes/node.hpp"



namespace Tank::Editor
{
	struct TabItem
	{
		std::string name;
		std::function<bool(const std::string &name)> getEnabled;
		std::function<void()> onSelect;
	};

	struct Tab
	{
		std::string name;
		std::function<bool(const std::string &name)> getEnabled;
		std::vector<TabItem> items;
	};


	class _MainMenuBar : public Node
	{
		friend class EditorApp;
	private:
		_MainMenuBar(const std::string &name, const std::vector<Tab> &tabs);

		std::vector<Tab> m_tabs;
	public:
		virtual ~_MainMenuBar() = default;

		virtual void draw() override;
	};
}