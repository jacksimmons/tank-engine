#pragma once
namespace Tank
{
	class TANK_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	public:
		static void init();

		static std::shared_ptr<spdlog::logger> &getCoreLogger() { return s_coreLogger; }
		static std::shared_ptr<spdlog::logger> &getClientLogger() { return s_clientLogger; }
	};
}


// Core log macros
#define TE_CORE_CRITICAL(...)	::Tank::Log::getCoreLogger()->critical(__VA_ARGS__)
#define TE_CORE_ERROR(...)		::Tank::Log::getCoreLogger()->error(__VA_ARGS__)
#define TE_CORE_WARN(...)		::Tank::Log::getCoreLogger()->warn(__VA_ARGS__)
#define TE_CORE_INFO(...)		::Tank::Log::getCoreLogger()->info(__VA_ARGS__)
#define TE_CORE_TRACE(...)		::Tank::Log::getCoreLogger()->trace(__VA_ARGS__)

#define TE_CRITICAL(...)		::Tank::Log::getClientLogger()->critical(__VA_ARGS__)
#define TE_ERROR(...)			::Tank::Log::getClientLogger()->error(__VA_ARGS__)
#define TE_WARN(...)			::Tank::Log::getClientLogger()->warn(__VA_ARGS__)
#define TE_INFO(...)			::Tank::Log::getClientLogger()->info(__VA_ARGS__)
#define TE_TRACE(...)			::Tank::Log::getClientLogger()->trace(__VA_ARGS__)