#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/epsilon.hpp>
#include <Log.h>
#include <static/Math.h>
#include <static/Constant.h>
#include <static/Time.h>
#include "PhysicsBody.h"


namespace Tank
{
	std::vector<PhysicsBodyComponent*> PhysicsBodyComponent::s_instances;


	PhysicsBodyComponent::PhysicsBodyComponent(float mass = 1) : m_mass(mass)
	{
		s_instances.push_back(this);
	}


	PhysicsBodyComponent::~PhysicsBodyComponent()
	{
		auto it = std::find(s_instances.begin(), s_instances.end(), this);
		if (it != s_instances.end())
			s_instances.erase(it);
		else
			TE_CORE_CRITICAL("Destructor: PhysicsBody was missing from s_instances.");
	}


	glm::vec3 PhysicsBodyComponent::getCentre() const noexcept
	{
		return glm::vec3(0.0f);
	}


	void PhysicsBodyComponent::update()
	{
		if (!m_started) return;

		// Ensure velocities list is correct size before starting
		while (s_instances.size() > m_velocities.size()) m_velocities.push_back({});
		float dt = Time::getFrameDelta();

		// Handle physics for all other physics bodies
		for (size_t i = 0; i < s_instances.size(); i++)
		{
			PhysicsBodyComponent *body = s_instances[i];
			if (body == this) continue;

			handleInteraction(i, dt);
		}

		glm::vec3 totalVelocity = {};
		for (const glm::vec3 &velocity : m_velocities)
		{
			totalVelocity += velocity;
		}

		(void)totalVelocity;

	}


	void PhysicsBodyComponent::handleInteraction(size_t bodyIndex, float dt)
	{
		PhysicsBodyComponent *other = s_instances[bodyIndex];
		glm::vec3 centre = getCentre();
		glm::vec3 otherCentre = other->getCentre();

		// Obtain positive force
		glm::vec3 sep = (otherCentre - centre);
		glm::vec3 force;

		// If separation is approx. 0, then set force to 0.
		if (glm::all(glm::epsilonEqual(sep, {}, Physics::EPSILON)))
			force = {};
		else
			force = glm::normalize(sep) * getGravityScalar(glm::length(sep), other->m_mass);

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


	float PhysicsBodyComponent::getGravityScalar(float distance, float otherMass) const
	{
		// F = Gm1m2/(r*r)
		float f = (Physics::G * m_mass * otherMass) / (distance * distance);
		return f;
	}
}
