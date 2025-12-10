#include <functional>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include "log.hpp"
#include "file.hpp"
#include "nodes/node.hpp"
#include "script.hpp"
#include "script_manager.hpp"


namespace Tank
{
	std::optional<std::unique_ptr<Script>> Script::createScript(Node *node, fs::path path)
	{
		// Try to load lua file
		std::filesystem::path scriptPath = std::filesystem::path() / "scripts" / path;
		ScriptData data = ScriptManager::addScript(scriptPath);
		if (data.getContents() == "") return {};

		// Interpret script with sol2
		Script *script = new Script(node, path, data.getContents());
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
		
		// Setup script functions
		Transform *transform = m_node->getTransform();
		{
			auto trans = transform->getLocalTranslation();
			auto table = lua.create_table_with(
				"x", trans.x,
				"y", trans.y,
				"z", trans.z
			);
			lua["translation"] = table;
		}

		// Call update function on lua script, if present
		std::optional<sol::protected_function> update = lua["update"];
		if (update.has_value())
			update.value()();

		transform->setLocalTranslation({ lua["translation"]["x"], lua["translation"]["y"], lua["translation"]["z"] });
	}
}