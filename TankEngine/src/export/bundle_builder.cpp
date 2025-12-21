#include <log.hpp>
#include <file.hpp>
#include <nodes/scene.hpp>
#include <scene_serialisation.hpp>
#include "bundle_builder.hpp"


namespace Tank::Export
{
	bool BundleBuilder::build(Scene *scene, const fs::path &path)
	{
		// Check if TankPlayer is built
		// @todo This should be generic; TankEngine shouldn't have a dependency on TankPlayer.
		bool playerExists = fs::exists(TANK_PLAYERDIR);
		if (!playerExists)
		{
			TE_CORE_ERROR("BundleBuilder > No TankPlayer executable found with configuration matching TankEngine.");
			return false;
		}

		// Serialise scene, and copy Player to the path (if empty)
		if (!fs::is_empty(path))
		{
			TE_CORE_ERROR(std::format("BundleBuilder > The target directory ({}) was not empty.", path.string()));
			return false;
		}

		TE_CORE_INFO("BundleBuilder > Saving scene...");
		Tank::Serialisation::saveScene(scene, path / "scene.json");
		prepareSerialisedData(path);

		TE_CORE_INFO("BundleBuilder > Copying assets...");
		fs::copy("shaders", path / "shaders", fs::copy_options::recursive);
		fs::copy("textures", path / "textures", fs::copy_options::recursive);
		fs::copy("models", path / "models", fs::copy_options::recursive);
		fs::copy("audio", path / "audio", fs::copy_options::recursive);
		fs::copy("scripts", path / "scripts", fs::copy_options::recursive);

		TE_CORE_INFO("BundleBuilder > Copying player...");
		fs::copy(TANK_PLAYERDIR, path);
		return true;
	}


	void BundleBuilder::prepareSerialisedData(const fs::path &path)
	{
		// Read the serialised file
		std::string serialised;
		auto result = File::readLines(path, serialised);

		// Cut all occurrences of project dir
		std::string projectDir = fs::current_path().string();
		while (serialised.contains(projectDir))
		{
			size_t index = serialised.find(projectDir);
			serialised.replace(index, projectDir.size(), "");
		}

		// Modify the serialised file
		File::writeLines(path, serialised);
	}
}