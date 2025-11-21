#pragma once
#include <optional>
#include "serialisation.hpp"


namespace Tank
{
	class Scene;
	namespace Serialisation
	{
		// Load a scene from disk, and gain ownership of it.
		TANK_API Scene* loadScene(const std::filesystem::path &scenePath);
		TANK_API void	saveScene(Scene *scene, const std::filesystem::path &scenePath);
	}
}