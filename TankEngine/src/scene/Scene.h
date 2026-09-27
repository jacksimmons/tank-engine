#pragma once
#include <entt/entt.hpp>


namespace Tank
{
	class Entity;
	class TANK_API Scene
	{
		friend class Entity;
	public:
		Scene();
		~Scene();

		Entity createEntity();
		void update();
	private:
		entt::registry m_registry;
	};
}