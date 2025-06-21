#pragma once
#include "nodes/node.hpp"
#include "shapes/collision_shape.hpp"


namespace Tank
{
	class TANK_API Collider : public Node
	{
		static std::vector<Collider*> s_instances;

	private:
		std::unique_ptr<CollisionShape> m_shape;

	public:
		Collider(const std::string &name, std::unique_ptr<CollisionShape> shape);
		virtual ~Collider();

		virtual void update() override;
	};
}