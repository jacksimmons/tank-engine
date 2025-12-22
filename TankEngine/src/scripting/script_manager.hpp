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
		ScriptData(const Res &path)
		{
			load(path);
		}

		void load(const Res &path);
		const std::string &getContents() const { return m_lastContents; }
	};

	typedef std::unordered_map<std::string, ScriptData> ScriptMap;

	/// <summary>
	/// Stores all loaded scripts.
	/// </summary>
	class TANK_API ScriptManager
	{
	private:
		static ScriptMap s_loadedScripts;
	public:
		static const ScriptMap &getScripts() { return s_loadedScripts; }

		static ScriptData addScript(const Res &path);
		static ScriptData updateScript(const Res &path);
		static bool popScript(const Res &path);
	};
}