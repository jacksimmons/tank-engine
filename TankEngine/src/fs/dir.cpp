#include <log.hpp>
#include "dir.hpp"


namespace Tank
{
	std::optional<std::error_code> Dir::tryCopy(const fs::path &from, const fs::path &to, fs::copy_options opts) noexcept
	{
		std::error_code ec;
		
		// Ensure we are copying directories
		if (!fs::is_directory(from) || !fs::is_directory(to))
			ec = std::make_error_code(std::errc::not_a_directory);
		else
			fs::copy(from, to, opts, ec);

		if (ec)
		{
			TE_CORE_ERROR(std::format("\"{}\" (Copying from path {} to {})", ec.message(), from.string(), to.string()));
			return ec;
		}

		return {};
	}
}