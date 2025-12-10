#pragma once
#include <log.hpp>
#include <file.hpp>


namespace Tank
{
	/// <summary>
	/// Stores all data required to load a script.
	/// </summary>
	class TANK_API ScriptData
	{
	private:
		std::string m_lastContents;
	public:
		ScriptData() : m_lastContents("") {}
		ScriptData(fs::path path)
		{
			load(path);
		}

		void load(fs::path path)
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
		const std::string &getContents() const { return m_lastContents; }
	};

	typedef std::unordered_map<fs::path, ScriptData> ScriptMap;

	/// <summary>
	/// Stores all loaded scripts.
	/// </summary>
	class TANK_API ScriptManager
	{
	private:
		static ScriptMap s_loadedScripts;
	public:
		static const ScriptMap &getScripts()
		{
			return s_loadedScripts;
		}

		static ScriptData addScript(fs::path path)
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

		static ScriptData updateScript(fs::path path)
		{
			s_loadedScripts[path].load(path);
			return s_loadedScripts[path];
		}

		static bool popScript(fs::path path)
		{
			if (s_loadedScripts.contains(path))
			{
				s_loadedScripts.erase(path);
				return true;
			}

			return false;
		}
	};
}