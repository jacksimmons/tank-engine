#pragma once
#include <optional>
#include <typeinfo>
#include <core.hpp>
#include "nodes/node.hpp"


namespace Tank
{
	namespace Serialisation
	{
		json serialise(Node *deserialised);
		Node* deserialise(const json &serialised);
	}
}