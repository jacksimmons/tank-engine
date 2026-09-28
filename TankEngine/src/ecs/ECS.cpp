#include "ECS.h"
#include "Entity.h"
#include <components/Innate.h>


namespace Tank
{
	ECS::ECS()
	{
	}


	ECS::~ECS()
	{
	}


	Entity ECS::createEntity()
	{
		Entity entity = { m_registry.create(), this };
		
		// ALL entities have these components.
		entity.addComponent<TransformComponent>();

		return entity;
	}


	void ECS::update()
	{
	}
}