#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/epsilon.hpp>
#include <components/PhysicsBody.h>
#include <Log.h>
#include "Physics.h"


namespace Tank
{
	float Physics::gravitation(float M, float m, float r)
	{
		// F = GMm/(r*r)
		float f = (PHYSICS::G * M * m) / (r * r);
		return f;
	}


	void Physics::handleGravity(const PhysicsBodyComponent &M, const PhysicsBodyComponent &m, float dt)
	{
		while (s_instances.size() > m_velocities.size()) m_velocities.push_back({});

		glm::vec3 M_centre = M.getCentre();
		glm::vec3 m_centre = m.getCentre();

		// Obtain positive force
		glm::vec3 sep = (m_centre - M_centre);
		glm::vec3 force;

		// If separation is approx. 0, then set force to 0.
		if (glm::all(glm::epsilonEqual(sep, {}, PHYSICS::EPSILON)))
			force = {};
		else
			force = glm::normalize(sep) * gravitation(M.Mass, m.Mass, glm::length(sep));

		// Apply F = dp / dt
		glm::vec3 changeInMomentum = force * dt;
		m_velocities[bodyIndex] += (changeInMomentum / m_mass);

		// Snap to other centre, if we would pass over it this frame
		if (glm::length(sep) < (glm::length(m_velocities[bodyIndex]) * dt))
		{
			TE_CORE_INFO("HI");

			m_velocities[bodyIndex] = {};
		}
	}
}