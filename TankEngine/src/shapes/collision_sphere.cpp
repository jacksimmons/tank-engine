#include "collision_sphere.hpp"
#include "transform.hpp"


namespace Tank
{
	bool CollisionSphere::contains(const glm::vec3 &point)
	{
		// Calculate radius of point from origin
		float pointRadius = glm::distance(m_transform->getLocalTranslation(), point);

		if (!m_isHollow) return pointRadius <= m_radius;
		else
		{
			float epsilon = 0.001f;
			return pointRadius - m_radius <= epsilon;
		}
	}
}