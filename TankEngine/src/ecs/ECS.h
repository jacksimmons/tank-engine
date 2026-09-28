#pragma once
#include <entt/entt.hpp>


namespace Tank
{
	class Entity;
	class TANK_API ECS
	{
		friend class Entity;
	public:
		ECS();
		~ECS();

		Entity createEntity();
		void update();
	private:
		entt::registry m_registry;
	};
}