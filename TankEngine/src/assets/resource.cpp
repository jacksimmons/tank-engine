#include <log.hpp>
#include "resource.hpp"


namespace Tank
{
	std::string Resource::s_engineToken = "{engine}";
	fs::path Resource::s_enginePath = fs::current_path();
	fs::path Resource::s_projectPath = fs::current_path();

	
	std::string Resource::encode(const Resource &res)
	{
		return (res.m_inEngine ? s_engineToken : "") + res.m_path.string();
	}


	Resource Resource::decode(const std::string &encoded)
	{
		bool inEngine = false;
		std::string path = encoded;
		if (encoded.contains(s_engineToken))
		{
			path.erase(encoded.find(s_engineToken), s_engineToken.size());
			inEngine = true;
		}

		return Resource(path, inEngine);
	}


	Resource::Resource(const fs::path &path, bool inEngine)
		: m_path(path), m_inEngine(inEngine) {}


	fs::path Resource::resolvePath() const
	{
		if (m_inEngine)
		{
			return s_enginePath / m_path;
		}
		else
		{
			return s_projectPath / m_path;
		}
	}


	std::string Resource::resolvePathStr() const
	{
		return resolvePath().string();
	}
}