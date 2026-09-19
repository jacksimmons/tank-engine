#pragma once


namespace Tank::File
{
	enum class ReadResult
	{
		Success,
		NotFile,
		Error
	};

	TANK_API bool exists(const std::filesystem::path &fp);
	TANK_API ReadResult readLines(const std::filesystem::path &fp, std::string &outStr) noexcept;
	TANK_API char* readBytes(const std::filesystem::path &fp, int *outSize);

	// @brief
	// Tries to create a copy of file `from` at path `to`.
	// Handles all exception logging, and returns the error code.
	TANK_API std::optional<std::error_code> tryCopy(
		const fs::path &from,
		const fs::path &to,
		fs::copy_options opts = fs::copy_options::none
	) noexcept;
}