#pragma once
#include <optional>
#include <typeinfo>
#include <core.hpp>
#include "nodes/node.hpp"


namespace Tank
{
	namespace Reflect { class NodeFactory; }
	namespace Serialisation
	{
		json serialise(Node *deserialised);
		Node* deserialise(const json &serialised, Reflect::NodeFactory *factory);
	}
}