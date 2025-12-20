#pragma once
#include <optional>
#include <memory>
#include <string>
#include "core.hpp"


namespace sol { class state; }
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
		fs::path m_scriptPath;
		std::string m_scriptLines;
		Script(Node *node, fs::path scriptPath, std::string scriptLines) : m_node(node), m_scriptPath(scriptPath), m_scriptLines(scriptLines) {}
	public:
		~Script() = default;

		static std::optional<std::unique_ptr<Script>> createScript(Node *node, fs::path path);

		void setEnabled(bool enabled) noexcept { m_enabled = enabled; }
		bool getEnabled() const noexcept { return m_enabled; }

		const fs::path &getScriptPath() const noexcept { return m_scriptPath; }

		void update();
		void pushProperties(sol::state &);
		void pullProperties(const sol::state &);
	};
}