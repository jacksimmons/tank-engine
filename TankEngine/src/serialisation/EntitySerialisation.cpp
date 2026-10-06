#include <scene/Entity.h>
#include <components/Components.h>
#include "EntitySerialisation.h"


namespace Tank
{
	void EntitySerialisation::serialise(json &out, const Entity &entity)
	{
		json serialised;

		if (entity.hasComponent<AudioComponent>())
		{
			^^
		}
	}
}