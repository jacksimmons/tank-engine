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
		Node* deserialise(const json &serialised)
		{
			// Pointer to a Node* or subclass of. 
			Node *node = nullptr;
			const std::string &type = serialised["type"];

			// +Node
			if (type == "Node") Node::deserialise(serialised, &node);
			else if (type == "Camera") Camera::deserialise(serialised, (Camera**)&node);
			else if (type == "CubeMap") CubeMap::deserialise(serialised, (CubeMap**)&node);
			else if (type == "DirLight") DirLight::deserialise(serialised, (DirLight**)&node);
			else if (type == "PointLight") PointLight::deserialise(serialised, (Light**)&node);
			else if (type == "Model") Model::deserialise(serialised, (Model**)&node);
			else if (type == "Scene") Scene::deserialise(serialised, (Scene**)&node);
			else if (type == "Sprite") Sprite::deserialise(serialised, (Sprite**)&node);
			else TE_CORE_CRITICAL("Unsupported type attempted to be deserialised: " + type);

			for (const json &child : serialised["children"].get<std::vector<json>>())
			{
				node->addChild(std::unique_ptr<Node>(deserialise(child)));
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