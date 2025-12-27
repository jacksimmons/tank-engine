#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <nodes/node.hpp>
#include <nodes/scene.hpp>
#include <nodes/camera.hpp>
#include <log.hpp>
#include "script.hpp"
#include "script_manager.hpp"
#include "user_types.hpp"


namespace Tank
{
	std::optional<std::unique_ptr<Script>> Script::createScript(Node *node, const Res &path)
	{
		// Try to load lua file
		ScriptData data = ScriptManager::addScript(path);
		if (data.getContents() == "") return {};

		// Interpret script with sol2
		Script *script = new Script(node, path, data.getContents());
		script->initSol();
		return std::unique_ptr<Script>(script);
	}


	void Script::initSol()
	{
		m_state.open_libraries(sol::lib::base, sol::lib::package);
		auto result = m_state.safe_script(m_scriptLines, sol::script_pass_on_error);
		if (!result.valid())
		{
			sol::error err = result;
			TE_CORE_ERROR(std::string("Script > Sol failed to init:\n") + err.what());
			return;
		}

		// Define usertypes
		UserTypes::generate(m_state);
		UserTypes::codegen();
	}


	void Script::update()
	{
		// Push properties to Lua from C++
		pushProperties();

		// Call update function on lua script, if present
		std::optional<sol::protected_function> update = m_state["Update"];
		if (update.has_value())
			update.value()();

		// Pull properties from Lua to C++
		pullProperties();
	}


	void Script::pushProperties()
	{
		m_state["node"] = m_node;
	}


	void Script::pullProperties()
	{
	}
}