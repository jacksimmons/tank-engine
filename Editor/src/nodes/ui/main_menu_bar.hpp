#pragma once
#include "nodes/node.hpp"



namespace Tank::Editor
{
	struct TabItem
	{
		const std::string name;
		const std::function<bool()> getEnabled;
		const std::function<void()> onSelect;
	};

	struct Tab
	{
		const std::string name;
		const std::function<bool()> getEnabled;
		const std::vector<TabItem> items;
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