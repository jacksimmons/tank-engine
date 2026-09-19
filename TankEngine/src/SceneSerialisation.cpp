#include <format>
#include <fstream>
#include "SceneSerialisation.h"
#include "fs/File.h"
#include "Log.h"
#include "nodes/Node.h"
#include "nodes/Camera.h"
#include "nodes/CubeMap.h"
#include "nodes/Light.h"
#include "nodes/Model.h"
#include "nodes/Scene.h"
#include "reflection/NodeFactory.h"


namespace Tank
{
	namespace Serialisation
	{
		_Scene* loadScene(const std::filesystem::path &scenePath, const Reflect::NodeFactory &factory)
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

			if (_Scene *scene = dynamic_cast<_Scene*>(deserialise(serialised, factory)))
			{
				return scene;
			}

			TE_CORE_ERROR("File selected was not a valid scene.");
			return nullptr;
		}


		void saveScene(_Scene *scene, const std::filesystem::path &scenePath)
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