#pragma once
#include <optional>
#include <typeinfo>
#include <Core.h>
#include "nodes/Node.h"


namespace Tank
{
	template <typename T>
	using Serialiser = ;

	template <typename T>
	using Deserialiser = T* (*)(const json &);

	namespace Reflect { class NodeFactory; }
	namespace Serialisation
	{
		json serialise(Node *deserialised);
		Node* deserialise(const json &serialised, const Reflect::NodeFactory &factory);
	}
}