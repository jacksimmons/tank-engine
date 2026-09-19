#pragma once
#include <entt/entt.hpp>


namespace Tank
{
	class TANK_API _Scene
	{
	public:
		_Scene();
		~_Scene();
	private:
		entt::registry m_registry;
	};
}