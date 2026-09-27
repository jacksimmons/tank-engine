#include <nodes/Node.h>
#include "Scene.h"
#include "Entity.h"


namespace Tank
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::createEntity()
	{
		Entity entity = { m_registry.create(), this };
		
		// ALL entities have these components.
		entity.addComponent<Transform>();

		return entity;
	}

	void Scene::update()
	{
	}
}