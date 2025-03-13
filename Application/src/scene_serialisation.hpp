#pragma once
#include <optional>
#include "serialisation.hpp"


namespace Tank
{
	class Scene;
	namespace Serialisation
	{
		// Load a scene from disk, and gain ownership of it.
		Scene* loadScene(const std::filesystem::path &scenePath);
		void saveScene(Scene *scene, const std::filesystem::path &scenePath);
	}
}