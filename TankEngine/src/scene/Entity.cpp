#include "Entity.h"
#include "Scene.h"


namespace Tank
{
	Entity::~Entity()
	{
		m_ecs->m_registry.destroy(m_handle);
	}
}