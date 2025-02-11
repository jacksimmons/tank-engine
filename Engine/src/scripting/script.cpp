#include <functional>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include <variant>
#include "log.hpp"
#include "file.hpp"
#include "json.hpp"
#include "serialisation.hpp"
#include "nodes/node.hpp"
#include "scripting/script.hpp"
#include "static/glm_serialise.hpp"


namespace Tank
{
	std::optional<std::unique_ptr<Script>> Script::createNewScript(Node *node, std::string filename)
	{
		std::string scriptsDir = std::string(ROOT_DIRECTORY) + "/scripts";
		std::string scriptPath = scriptsDir + "/" + filename;

		// Check if the filename provided exists
		if (File::exists(scriptPath))
		{
			TE_CORE_INFO(std::string("Script file already exists: ") + scriptPath);
		}
		
		// Copy the template
		std::string newScriptLines;
		if (!File::readLines(scriptsDir + "/template.lua", newScriptLines))
		{
			TE_CORE_ERROR("Failed to read template script.");
			return {};
		}
		
		// Write the copied template into new file
		if (!File::writeLines(scriptPath, newScriptLines))
		{
			TE_CORE_ERROR("Failed to create new script.");
			return {};
		}

		return std::unique_ptr<Script>(new Script(node, filename, newScriptLines));
	}


	std::optional<std::unique_ptr<Script>> Script::createExistingScript(Node *node, std::string filename)
	{
		// Load script (.lua) file
		std::string scriptLines;
		std::filesystem::path scriptPath = std::filesystem::path(ROOT_DIRECTORY) / "Scripts" / filename;
		if (!File::readLines(scriptPath, scriptLines))
		{
			TE_CORE_ERROR(std::string("Script did not exist: ") + scriptPath.string());
			return {};
		}

		Script *script = new Script(node, filename, scriptLines);
		return std::unique_ptr<Script>(script);
	}


	void Script::update()
	{
		// ! Remove this !
		std::filesystem::path scriptPath = std::filesystem::path(ROOT_DIRECTORY) / "Scripts" / m_filename;
		if (!File::readLines(scriptPath, m_scriptLines))
		{
			TE_CORE_ERROR(std::string("Script did not exist: ") + scriptPath.string());
		}

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
		{
			auto rot = glm::eulerAngles(transform->getLocalRotation());
			auto table = lua.create_table_with(
				"x", rot.x,
				"y", rot.y,
				"z", rot.z
			);
			lua["rotation"] = table;
		}

		// Call update function on lua script, if present
		std::optional<sol::protected_function> update = lua["update"];
		if (update.has_value())
			update.value()();

		transform->setLocalTranslation({ lua["translation"]["x"], lua["translation"]["y"], lua["translation"]["z"] });
		transform->setLocalRotation(quat::fromAngleAxis({ lua["rotation"]["x"], lua["rotation"]["y"], lua["rotation"]["z"] }));
	}
}