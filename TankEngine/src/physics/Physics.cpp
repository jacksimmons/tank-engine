#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/epsilon.hpp>
#include <scene/Entity.h>
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


	void Physics::handleGravity(entt::entity e, const PhysicsBodyComponent &M, const PhysicsBodyComponent &m, float dt)
	{
		if (!s_velocities.contains(e))
		{
			// Add a (0,0,0) velocity vector for e, if e hasn't been touched by physics yet.
			s_velocities.insert(std::make_pair(e, glm::vec3 { 0 }));
		}

		glm::vec3 M_centre = M.getCentre();
		glm::vec3 m_centre = m.getCentre();

		// Obtain positive force
		glm::vec3 sep = (m_centre - M_centre);
		glm::vec3 force;

		// If separation is approx. 0, then set force to 0.
		if (glm::all(glm::epsilonEqual(sep, {}, PHYSICS::EPSILON)))
			force = {};
		else
			force = glm::normalize(sep) * gravitation(M.mass, m.mass, glm::length(sep));

		// Apply F = dp / dt
		glm::vec3 changeInMomentum = force * dt;
		s_velocities[e] += (changeInMomentum / m.mass);

		// Snap to other centre, if we are going to pass over it this frame
		if (glm::length(sep) < (glm::length(s_velocities[e]) * dt))
		{
			s_velocities[e] = glm::vec3 { 0 };
		}
	}
}