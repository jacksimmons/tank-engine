#include <regex>
#include <log.hpp>
#include <file.hpp>
#include <string.hpp>
#include <nodes/scene.hpp>
#include <project/project.hpp>
#include <scene_serialisation.hpp>
#include "export.hpp"


namespace Tank
{
	static void copyProjectDir(const fs::path &src, const fs::path &dest)
	{
		// Exit if the src doesn't exist, or is not a directory
		if (!fs::exists(src) || !fs::is_directory(src)) return;

		// Ensure the dest exists, make it if not
		fs::create_directories(dest);

		// Copy recursively
		fs::copy(src, dest, fs::copy_options::recursive);
	}


	bool Export::project(Project *project, const fs::path &path)
	{
		// Check if TankPlayer is built
		// @todo This should be generic; TankEngine shouldn't have a dependency on TankPlayer.
		bool playerExists = fs::exists(TANK_PLAYERDIR) && !fs::is_empty(TANK_PLAYERDIR);
		if (!playerExists)
		{
			TE_CORE_ERROR("Export > No TankPlayer executable found with configuration matching TankEngine.");
			return false;
		}

		// Serialise scene, and copy Player to the path (if empty)
		if (!fs::is_empty(path))
		{
			TE_CORE_ERROR(std::format("Export > The target directory ({}) was not empty.", path.string()));
			return false;
		}

		TE_CORE_INFO("Export > Copying player...");
		fs::copy(TANK_PLAYERDIR, path);

		TE_CORE_INFO("Export > Copying assets...");
		fs::path proj = fs::current_path();
		copyProjectDir(proj / "assets", path / "assets");

		TE_CORE_INFO("Export > Done.");
		return true;
	}
}