#pragma once


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

		void load(fs::path path);
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
		static const ScriptMap &getScripts() { return s_loadedScripts; }

		static ScriptData addScript(fs::path path);
		static ScriptData updateScript(fs::path path);
		static bool popScript(fs::path path);
	};
}