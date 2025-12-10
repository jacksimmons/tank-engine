#pragma once
#include <optional>
#include <memory>
#include <string>
#include "core.hpp"


namespace Tank
{
	class Node;

	/// <summary>
	/// A script which can be attached to a Node subclass.
	/// Binds to a lua script file, which can in turn access some properties
	/// of the Script object, such as its node.
	/// </summary>
	class TANK_API Script final
	{
	private:
		bool m_enabled = true;
		Node *m_node;
		std::string m_filename;
		std::string m_scriptLines;
		Script(Node *node, std::string filename, std::string scriptLines) : m_node(node), m_filename(filename), m_scriptLines(scriptLines) {}
	public:
		~Script() = default;

		static std::optional<std::unique_ptr<Script>> createScript(Node *node, std::string filename);

		void setEnabled(bool enabled) noexcept { m_enabled = enabled; }
		bool getEnabled() const noexcept { return m_enabled; }
		void update();
	};
}