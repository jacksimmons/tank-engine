#pragma once


namespace Tank
{
	namespace File
	{
		enum class ReadResult
		{
			Success,
			NoFile,
			Error
		};

		TANK_API bool exists(const std::filesystem::path &fp);
		TANK_API ReadResult readLines(const std::filesystem::path &fp, std::string &outStr);
		TANK_API bool writeLines(const std::filesystem::path &fp, const std::string &inStr);
		TANK_API char* readBytes(const std::filesystem::path &fp, int *outSize);
	}
}