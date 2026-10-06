#include "CollisionSphere.h"


namespace Tank
{
	bool CollisionSphere::contains(const glm::vec3 &point)
	{
		// Calculate radius of point from origin
		float pointRadius = glm::distance(m_transform.Translation, point);

		if (!m_isHollow) return pointRadius <= m_radius;
		else
		{
			float epsilon = 0.001f;
			return pointRadius - m_radius <= epsilon;
		}
	}
}