#pragma once


namespace Tank
{
	class TANK_API Resource final
	{
		friend class Application;
	private:
		static std::string s_engineToken;
		static fs::path s_enginePath;
		static fs::path s_projectPath;

		/// <summary>
		/// Path of the resource (relative to engine/project root).
		/// </summary>
		fs::path m_path;

		/// <summary>
		/// If `true`, then we use fs::current_path() / m_path (and need to copy it over if it's Engine during export).
		/// If `false`, then we use project_path / m_path.
		/// </summary>
		bool m_inEngine;
	public:

		static std::string encode(const Resource &decoded);
		static Resource decode(const std::string &encoded);
		static const fs::path &getProjPath() { return s_projectPath; }

		Resource(const fs::path &path = "", bool inEngine = false);
		bool operator==(const Resource &other) const
		{
			return Resource::encode(*this) == Resource::encode(other);
		}

		fs::path resolvePath() const;
		std::string resolvePathStr() const;
	};

	typedef Resource Res;
}