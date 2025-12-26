#include <regex>
#include <log.hpp>
#include <file.hpp>
#include <string.hpp>
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
		// Read the serialised file into a new Scene instance
		std::string serialised;
		std::string scenePath = (path / "scene.json").string();
		auto result = File::readLines(path / "scene.json", serialised);
		if (result != File::ReadResult::Success)
		{
			TE_CORE_ERROR(std::format("BundleBuilder > Error reading {}.", scenePath));
			return;
		}

		// Cut all occurrences of project dir; replace with relative dir
		// @todo Neaten this up once projects are enforced, and all assets are already required to be a subdirectory of the root project.
		// C:\\tank-engine
		std::string projectDir = std::regex_replace(
			fs::current_path().string(),
			std::regex(R"(\\)"),
			"/"
		);
		// C:/tank-engine\\...
		String::eraseAll(serialised, projectDir + "\\\\", 10000);
		// C:/tank-engine\...
		String::eraseAll(serialised, projectDir + "\\", 10000);
		// C:/tank-engine/...
		String::eraseAll(serialised, projectDir + "/", 10000);
		// C:/tank-engine...
		String::eraseAll(serialised, projectDir, 10000);

		projectDir = std::regex_replace(
			fs::current_path().string(),
			std::regex(R"(\\)"),
			R"(\\)"
		);
		// C:\\tank-engine\\...
		String::eraseAll(serialised, projectDir + "\\\\", 10000);
		// C:\\tank-engine\...
		String::eraseAll(serialised, projectDir + "\\", 10000);
		// C:\\tank-engine/...
		String::eraseAll(serialised, projectDir + "/", 10000);
		// C:\\tank-engine...
		String::eraseAll(serialised, projectDir, 10000);

		// Modify the serialised file
		bool status = File::writeLines(scenePath, serialised);
		if (!status)
		{
			TE_CORE_ERROR(std::format("BundleBuilder > Error writing to {}.", scenePath));
		}
	}
}