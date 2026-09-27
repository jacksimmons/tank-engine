#include "Entity.h"


namespace Tank
{
	Entity::Entity(const entt::entity handle, Scene *scene)
		: m_handle(handle), m_scene(scene)
	{
	}
}