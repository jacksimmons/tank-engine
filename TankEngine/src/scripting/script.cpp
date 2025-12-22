#include <functional>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include <nodes/node.hpp>
#include <nodes/scene.hpp>
#include <nodes/camera.hpp>
#include "log.hpp"
#include "script.hpp"
#include "script_manager.hpp"
#include "user_types.hpp"


namespace Tank
{
	std::optional<std::unique_ptr<Script>> Script::createScript(Node *node, const Res &path)
	{
		// Try to load lua file
		ScriptData data = ScriptManager::addScript(path.resolvePath());
		if (data.getContents() == "") return {};

		// Interpret script with sol2
		Script *script = new Script(node, path.resolvePath(), data.getContents());
		return std::unique_ptr<Script>(script);
	}


	void Script::update()
	{
		// Setup lua script
		sol::state lua;
		lua.open_libraries(sol::lib::base, sol::lib::package);
		auto result = lua.safe_script(m_scriptLines, sol::script_pass_on_error);
		if (!result.valid())
		{
			sol::error err = result;
			TE_CORE_ERROR(std::string("Script file failed to run:\n") + err.what());
		}

		// Define usertypes
		UserTypes::all(lua);

		// Push properties to Lua from C++
		pushProperties(lua);

		// Call update function on lua script, if present
		std::optional<sol::protected_function> update = lua["update"];
		if (update.has_value())
			update.value()();

		// Pull properties from Lua to C++
		pullProperties(lua);
	}


	void Script::pushProperties(sol::state &lua)
	{
		lua["node"] = m_node;
		lua["camera"] = Scene::getActiveScene()->getActiveCamera();
	}


	void Script::pullProperties(const sol::state &lua)
	{
	}
}