namespace Tank::FS
{
	// @brief
	// Tries to copy one location to another location.
	// Handles all exception logging, and returns the error code.
	TANK_API std::optional<std::error_code> tryCopy(
		const fs::path& from,
		const fs::path& to,
		fs::copy_options opts = fs::copy_options::none
	) noexcept;
}