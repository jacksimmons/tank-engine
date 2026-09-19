#pragma once


namespace Tank
{
	namespace String
	{
		TANK_API void eraseAll(std::string &str, const std::string &exp, size_t maxCount=1000);

		/// @brief Returns an upper-case copy.
		TANK_API std::string toUpper(const std::string &original);
	}
}