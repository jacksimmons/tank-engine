#include <log.hpp>
#include "dir.hpp"


namespace Tank
{
	std::optional<std::error_code> Dir::tryCopy(const fs::path &src, const fs::path &dest, fs::copy_options opts) noexcept
	{
		std::error_code ec;

		// Invalid src -> error
		if (!fs::is_directory(src))
		{
			ec = std::make_error_code(std::errc::not_a_directory);
		}

		// No dest -> try making dest
		if (!fs::exists(dest))
		{
			fs::create_directories(dest, ec);
		}

		// No errors so far -> try copying
		if (!ec)
		{
			fs::copy(src, dest, opts, ec);
		}

		// Output and return any error codes
		if (ec)
		{
			TE_CORE_ERROR(std::format("\"{}\" (Copying from path {} to {})", ec.message(), src.string(), dest.string()));
			return ec;
		}

		return {};
	}
}