#include "Entity.h"
#include <components/Transform.h>
#include <components/Tree.h>
#include <events/EventManager.h>
#include <KeyInput.h>


namespace Tank
{
	Entity::~Entity()
	{
		m_ecs->m_registry.destroy(m_handle);
	}
}