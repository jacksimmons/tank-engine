#pragma once
#include <optional>
#include "Serialisation.h"


namespace Tank
{
	class _Scene;
	namespace Reflect { class NodeFactory; }
	namespace Serialisation
	{
		// Load a scene from disk, and gain ownership of it.
		TANK_API _Scene* loadScene(const std::filesystem::path &scenePath, const Reflect::NodeFactory &factory);
		TANK_API void saveScene(_Scene *scene, const std::filesystem::path &scenePath);
	}
}