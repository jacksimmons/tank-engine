#include <typeinfo>
#include <nodes/node.hpp>
#include "node_factory.hpp"


namespace Tank::Reflect
{
	Node *NodeFactory::deserialise(const json &serialised)
	{
		// Deduce type
		std::string type = serialised["type"];

		auto classType = tryGetClass(type);
		if (!classType.has_value())
		{
			TE_CORE_CRITICAL(std::format("Failed to deserialise type with name {}: it was not registered to a class.", type));
			return nullptr;
		}

		constructor_t constructor = classType.value();

		Node *node = (Node *)constructor();
		node->deserialise(serialised);
		return node;
	}
}