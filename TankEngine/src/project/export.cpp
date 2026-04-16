#include <regex>
#include <log.hpp>
#include <fs/file.hpp>
#include <string.hpp>
#include <nodes/scene.hpp>
#include <project/project.hpp>
#include <scene_serialisation.hpp>
#include "export.hpp"
#include <fs/dir.hpp>


namespace Tank
{
	bool Export::project(const Project &project, const fs::path &path)
	{
		// Assert TankPlayer for current configuration is built
		assert(fs::exists(TANK_PLAYERDIR));
		assert(!fs::is_empty(TANK_PLAYERDIR));

		// Serialise scene, and copy Player to the path (if empty)
		if (!fs::is_empty(path))
		{
			TE_CORE_ERROR(std::format("Export > The target directory ({}) was not empty.", path.string()));
			return false;
		}

		TE_CORE_INFO("Export > Copying engine...");
		Dir::tryCopy(TANK_ENGINEDIR, path, fs::copy_options::update_existing);
		
		TE_CORE_INFO("Export > Copying player...");
		Dir::tryCopy(TANK_PLAYERDIR, path, fs::copy_options::update_existing);

#ifdef __linux__
		TE_CORE_INFO("Export > Patching player...");
		system(
			std::format(
				"patchelf --set-rpath '$ORIGIN' {}",
				(path / "TankPlayer").string()
			).c_str()
		);
#endif
		
		TE_CORE_INFO("Export > Copying assets...");
		fs::path proj = fs::current_path();
		Dir::tryCopy(proj / "assets", path / "assets", fs::copy_options::recursive);

		TE_CORE_INFO("Export > Copying core assets...");
		Dir::tryCopy(Res::getCoreAssetsPath(), path / "CoreAssets", fs::copy_options::recursive);

		TE_CORE_INFO("Export > Done.");
		return true;
	}
}