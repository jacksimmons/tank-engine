#pragma once
#include "collision_shape.hpp"


namespace Tank
{
	class Transform;
	class TANK_API CollisionSphere : public CollisionShape
	{
	private:
		Transform *m_transform;
		float m_radius;
		bool m_isHollow;

	public:
		CollisionSphere(Transform *transform, float radius, bool isHollow=false)
			: m_transform(transform), m_radius(radius), m_isHollow(isHollow), CollisionShape() {}
		virtual ~CollisionSphere() = default;

		virtual bool contains(const glm::vec3 &point) override;
	};
}