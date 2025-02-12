#pragma once
#include <memory>
#include <filesystem>
#include <optional>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include "nodes/scene.hpp"
#include "serialisation.hpp"

using json = nlohmann::json;


namespace Tank
{
	namespace Serialisation
	{
		// Load a scene from disk, and gain ownership of it.
		Scene* loadScene(const std::filesystem::path &scenePath);
		void saveScene(Scene *scene, const std::filesystem::path &scenePath);
	}
}