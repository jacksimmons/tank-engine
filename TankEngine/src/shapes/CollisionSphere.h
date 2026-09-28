#pragma once
#include "CollisionShape.h"


namespace Tank
{
	class TransformComponent;
	class TANK_API CollisionSphere : public CollisionShape
	{
	private:
		const TransformComponent &m_transform;
		float m_radius;
		bool m_isHollow;

	public:
		CollisionSphere(const TransformComponent &transform, float radius, bool isHollow=false)
			: m_transform(transform), m_radius(radius), m_isHollow(isHollow), CollisionShape() {}
		virtual ~CollisionSphere() = default;

		virtual bool contains(const glm::vec3 &point) override;
	};
}