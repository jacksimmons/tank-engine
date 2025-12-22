#include <file.hpp>
#include <log.hpp>
#include "script_manager.hpp"


namespace Tank
{
	ScriptMap ScriptManager::s_loadedScripts;


	// =-=-=-=- ScriptData -=-=-=-=


	void ScriptData::load(const Res &path)
	{
		auto result = File::readLines(path.resolvePath(), m_lastContents);
		if (result != File::ReadResult::Success)
		{
			switch (result)
			{
			case File::ReadResult::Error:
				TE_CORE_ERROR(std::string("Error when loading script: ") + Res::encode(path));
				break;
			case File::ReadResult::NotFile:
				TE_CORE_ERROR(std::string("Script did not exist: ") + Res::encode(path));
				break;
			}

			m_lastContents = "";
		}
	}


	// =-=-=-=- ScriptManager -=-=-=-=


	ScriptData ScriptManager::addScript(const Res &path)
	{
		std::string encoded = Res::encode(path);
		if (s_loadedScripts.contains(encoded))
		{
			TE_CORE_WARN(std::format("Script already exists at {}, updating it...", encoded));
			return updateScript(path);
		}

		auto data = ScriptData(path);
		s_loadedScripts.insert(std::make_pair(encoded, data));
		return data;
	}


	ScriptData ScriptManager::updateScript(const Res &path)
	{
		std::string encoded = Res::encode(path);
		s_loadedScripts[encoded].load(path);
		return s_loadedScripts[encoded];
	}


	bool ScriptManager::popScript(const Res &path)
	{
		std::string encoded = Res::encode(path);
		if (s_loadedScripts.contains(encoded))
		{
			s_loadedScripts.erase(encoded);
			return true;
		}

		return false;
	}
}