#pragma once
#include <optional>
#include <typeinfo>
#include <Core.h>
#include "nodes/Node.h"


namespace Tank
{
	namespace Reflect { class NodeFactory; }
	namespace Serialisation
	{
		json serialise(Node *deserialised);
		Node* deserialise(const json &serialised, const Reflect::NodeFactory &factory);
	}
}