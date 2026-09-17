#pragma once
#include <optional>
#include <typeinfo>
#include <Core.hpp>
#include "nodes/Node.hpp"


namespace Tank
{
	namespace Reflect { class NodeFactory; }
	namespace Serialisation
	{
		json serialise(Node *deserialised);
		Node* deserialise(const json &serialised, const Reflect::NodeFactory &factory);
	}
}