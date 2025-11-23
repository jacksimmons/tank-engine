#include <format>
#include "scene_serialisation.hpp"
#include "file.hpp"
#include "log.hpp"
#include "nodes/node.hpp"
#include "nodes/camera.hpp"
#include "nodes/cube_map.hpp"
#include "nodes/light.hpp"
#include "nodes/model.hpp"
#include "nodes/scene.hpp"
#include "reflection/node_factory.hpp"


namespace Tank
{
	namespace Serialisation
	{
		Scene* loadScene(const std::filesystem::path &scenePath, Reflect::NodeFactory *factory)
		{
			std::string sceneFile;
			if (File::readLines(scenePath, sceneFile) != File::ReadResult::Success)
			{
				TE_CORE_ERROR(std::format("Failed to deserialise from file {}", scenePath.string()));
				return nullptr;
			}

			json serialised = json::parse(sceneFile);
			
			if (Scene *scene = dynamic_cast<Scene*>(deserialise(serialised, factory)))
			{
				return scene;
			}

			TE_CORE_ERROR("File selected was not a valid scene.");
			return nullptr;
		}


		void saveScene(Scene *scene, const std::filesystem::path &scenePath)
		{
			std::string sceneFile;
			if (File::readLines(scenePath, sceneFile) == File::ReadResult::Error)
			{
				TE_CORE_ERROR(std::format("Failed to serialise to file {}", scenePath.string()));
			}

			// Write with pretty print (indent=4)
			File::writeLines(scenePath, serialise(scene).dump(4));
		}
	}
}