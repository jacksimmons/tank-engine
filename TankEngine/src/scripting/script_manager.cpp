#include <file.hpp>
#include <log.hpp>
#include "script_manager.hpp"


namespace Tank
{
	ScriptMap ScriptManager::s_loadedScripts;


	// =-=-=-=- ScriptData -=-=-=-=


	void ScriptData::load(fs::path path)
	{
		auto result = File::readLines(path, m_lastContents);
		if (result != File::ReadResult::Success)
		{
			switch (result)
			{
			case File::ReadResult::Error:
				TE_CORE_ERROR(std::string("Error when loading script: ") + path.string());
				break;
			case File::ReadResult::NoFile:
				TE_CORE_ERROR(std::string("Script did not exist: ") + path.string());
				break;
			}

			m_lastContents = "";
		}
	}


	// =-=-=-=- ScriptManager -=-=-=-=


	ScriptData ScriptManager::addScript(fs::path path)
	{
		if (s_loadedScripts.contains(path))
		{
			TE_CORE_WARN(std::format("Script already exists at {}, updating it...", path.string()));
			return updateScript(path);
		}

		auto data = ScriptData(path);
		s_loadedScripts.insert(std::make_pair(path, data));
		return data;
	}


	ScriptData ScriptManager::updateScript(fs::path path)
	{
		s_loadedScripts[path].load(path);
		return s_loadedScripts[path];
	}


	bool ScriptManager::popScript(fs::path path)
	{
		if (s_loadedScripts.contains(path))
		{
			s_loadedScripts.erase(path);
			return true;
		}

		return false;
	}
}