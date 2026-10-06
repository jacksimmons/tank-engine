#include <format>
#include <fstream>
#include <reflection/ReflectionRegistry.h>
#include <scene/Scene.h> 
#include "SceneSerialisation.h"
#include "fs/File.h"
#include "Log.h"
#include "scene/Entity.h"
#include "components/Camera.h"
#include "components/CubeMap.h"
#include "components/Light.h"
#include "components/Model.h"


namespace Tank
{
	namespace Serialisation
	{
		Scene* loadScene(const std::filesystem::path &scenePath)
		{
			std::string sceneFile;
			if (File::readLines(scenePath, sceneFile) != File::ReadResult::Success)
			{
				TE_CORE_ERROR(std::format("Failed to deserialise from file {}", scenePath.string()));
				return nullptr;
			}

			json serialised;
			try
			{
				serialised = json::parse(sceneFile);
			}
			catch (std::exception e)
			{
				TE_CORE_ERROR(std::format("Couldn't parse {} into JSON.", scenePath.string()));
				return nullptr;
			}

			if (Scene *scene = ReflectionRegistry::deserialise<Scene>(serialised))
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
			TE_CORE_INFO(std::format("Saving scene to {}", scenePath.string()));

			std::string dump = serialise(scene).dump(4);
			std::ofstream out(scenePath);
			
			if (!out)
			{
				TE_CORE_ERROR("Failed to serialise scene at file {}", scenePath.string());
				return;
			}
			
			out.write(dump.c_str(), dump.size());
		}
	}
}
