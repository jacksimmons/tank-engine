namespace Tank::Dir
{
	// @brief
	// Tries to copy the contents of one directory into another.
	// Handles all exception logging, and returns the error code.
	TANK_API std::optional<std::error_code> tryCopy(
		const fs::path& from,
		const fs::path& to,
		fs::copy_options opts = fs::copy_options::none
	) noexcept;
}