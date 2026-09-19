#include <format>
#include "Serialisation.h"
#include "fs/File.h"
#include "Log.h"
#include "nodes/Camera.h"
#include "nodes/CubeMap.h"
#include "nodes/Light.h"
#include "nodes/Model.h"
#include "nodes/Scene.h"
#include "nodes/Sprite.h"
#include "reflection/NodeFactory.h"


namespace Tank
{
	namespace Serialisation
	{
		/// <summary>
		/// Serialises generic Node into json.
		/// </summary>
		json serialise(Node *deserialised)
		{
			json serialised = deserialised->serialise();

			std::vector<json> children;
			for (auto &child : *(Node*)deserialised)
			{
				children.push_back(serialise(child.get()));
			}
			serialised["children"] = children;

			return serialised;
		}


		/// <summary>
		/// Deserialises generic Node json into a Node.
		/// Allocations: +Node (return value).
		/// </summary>
		Node* deserialise(const json &serialised, const Reflect::NodeFactory &factory)
		{
			// Pointer to a Node* or subclass of. 
			Node *node = nullptr;
			const std::string &type = serialised["type"];

			node = factory.deserialise(serialised);
			for (const json &child : serialised["children"].get<std::vector<json>>())
			{
				node->addChild(std::unique_ptr<Node>(deserialise(child, factory)));
			}

			// Post-tree instantiation (after all children have been deserialised)
			if (type == "Scene")
			{
				_Scene *scene = (_Scene*)node;
				scene->preupdate();
				scene->setActiveCamera((Camera*)scene->childFromTree(serialised["activeCam"]));
			}

			return node;
		}
	}
}