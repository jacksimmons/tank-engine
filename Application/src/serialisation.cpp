#include <format>
#include "serialisation.hpp"
#include "file.hpp"
#include "log.hpp"
#include "nodes/camera.hpp"
#include "nodes/cube_map.hpp"
#include "nodes/light.hpp"
#include "nodes/model.hpp"
#include "nodes/scene.hpp"


namespace Tank
{
	namespace Serialisation
	{
		json serialise(Node *deserialised)
		{
			json serialised;
			const std::string &type = ((Node*)deserialised)->getType();

			if (type == "Node") serialised = Node::serialise((Node*)deserialised);
			else if (type == "Camera") serialised = Camera::serialise((Camera*)deserialised);
			else if (type == "CubeMap") serialised = CubeMap::serialise((CubeMap*)deserialised);
			else if (type == "DirLight") serialised = DirLight::serialise((DirLight*)deserialised);
			else if (type == "PointLight") serialised = PointLight::serialise((Light*)deserialised);
			else if (type == "Model") serialised = Model::serialise((Model*)deserialised);
			else if (type == "Scene") serialised = Scene::serialise((Scene*)deserialised);
			else serialised = Node::serialise((Node*)deserialised);

			std::vector<json> children;
			for (auto &child : *(Node*)deserialised)
			{
				children.push_back(serialise(child.get()));
			}
			serialised["children"] = children;

			return serialised;
		}


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
			else Node::deserialise(serialised, &node);

			for (const json &child : serialised["children"].get<std::vector<json>>())
			{
				node->addChild(std::unique_ptr<Node>(deserialise(child)));
			}

			// Post-tree instantiation (after all children have been deserialised)
			if (type == "Scene")
			{
				Scene *scene = (Scene*)node;
				scene->setActiveCamera((Camera*)scene->childFromTree(serialised["activeCam"]));
			}

			return node;
		}
	}
}