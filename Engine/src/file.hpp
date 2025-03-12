#pragma once

#include <fstream>
#include <exception>
#include <filesystem>
#include "core.hpp"


namespace Tank
{
	namespace File
	{
		TANK_API bool exists(const std::filesystem::path &fp);
		TANK_API bool readLines(const std::filesystem::path &fp, std::string &outStr);
		TANK_API bool writeLines(const std::filesystem::path &fp, const std::string &inStr);
		TANK_API char* readBytes(const std::filesystem::path &fp, int *outSize);
	}
}