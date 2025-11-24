#include <format>
#include "serialisation.hpp"
#include "file.hpp"
#include "log.hpp"
#include "nodes/camera.hpp"
#include "nodes/cube_map.hpp"
#include "nodes/light.hpp"
#include "nodes/model.hpp"
#include "nodes/scene.hpp"
#include "nodes/sprite.hpp"
#include "reflection/node_factory.hpp"


namespace Tank
{
	namespace Serialisation
	{
		/// <summary>
		/// Serialises generic Node into json.
		/// </summary>
		json serialise(Node *deserialised)
		{
			json serialised;
			const std::string &type = ((Node*)deserialised)->getType();

			if (type == "Node") serialised = ((Node*)deserialised)->serialise();
			else if (type == "Camera") serialised = ((Camera*)deserialised)->serialise();
			else if (type == "CubeMap") serialised = ((CubeMap*)deserialised)->serialise();
			else if (type == "DirLight") serialised = ((DirLight*)deserialised)->serialise();
			else if (type == "PointLight") serialised = ((Light*)deserialised)->serialise();
			else if (type == "Model") serialised = ((Model*)deserialised)->serialise();
			else if (type == "Scene") serialised = ((Scene*)deserialised)->serialise();
			else if (type == "Sprite") serialised = ((Sprite*)deserialised)->serialise();
			else TE_CORE_CRITICAL("Unsupported type attempted to be serialised: " + type);

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
		Node* deserialise(const json &serialised, Reflect::NodeFactory *factory)
		{
			// Pointer to a Node* or subclass of. 
			Node *node = nullptr;
			const std::string &type = serialised["type"];

			// +Node
			if (type == "CubeMap") CubeMap::deserialise(serialised, (CubeMap**)&node);
			else node = factory->create(serialised);

			for (const json &child : serialised["children"].get<std::vector<json>>())
			{
				node->addChild(std::unique_ptr<Node>(deserialise(child, factory)));
			}

			// Post-tree instantiation (after all children have been deserialised)
			if (type == "Scene")
			{
				Scene *scene = (Scene*)node;
				scene->preupdate();
				scene->setActiveCamera((Camera*)scene->childFromTree(serialised["activeCam"]));
			}

			return node;
		}
	}
}