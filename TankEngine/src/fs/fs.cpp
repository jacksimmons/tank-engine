#include <log.hpp>
#include "fs.hpp"


namespace Tank
{
	std::optional<std::error_code> FS::tryCopy(const fs::path &from, const fs::path &to, fs::copy_options opts) noexcept
	{
		std::error_code ec;
		fs::copy(from, to, opts, ec);
		if (ec)
		{
			TE_CORE_ERROR(std::format("{} when copying from path {} to {}", ec.message(), from.string(), to.string()));
			return ec;
		}

		return {};
	}
}