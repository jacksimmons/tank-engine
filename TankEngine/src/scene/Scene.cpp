#include <nodes/Node.h>
#include "Scene.h"


namespace Tank
{
	_Scene::_Scene()
	{
		entt::entity entity = m_registry.create();
		m_registry.emplace<Node>(entity);
	}

	_Scene::~_Scene()
	{
	}
}