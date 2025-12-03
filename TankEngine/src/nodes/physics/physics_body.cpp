#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/epsilon.hpp>
#include "log.hpp"
#include "physics_body.hpp"
#include "static/math.hpp"
#include "static/constant.hpp"
#include "static/time.hpp"


namespace Tank
{
	std::vector<PhysicsBody*> PhysicsBody::s_instances;


	PhysicsBody::PhysicsBody(const std::string &name, float mass)
		: Node(name), IMass(mass)
	{
		s_instances.push_back(this);
	}


	PhysicsBody::~PhysicsBody()
	{
		auto it = std::find(s_instances.begin(), s_instances.end(), this);
		if (it != s_instances.end())
			s_instances.erase(it);
		else
			TE_CORE_CRITICAL("Destructor: PhysicsBody was missing from s_instances.");
	}


	void PhysicsBody::update()
	{
		if (!m_started) return Node::update();

		// Ensure velocities list is correct size before starting
		while (s_instances.size() > m_velocities.size()) m_velocities.push_back({});
		float dt = Time::getFrameDelta();

		// Handle physics for all other physics bodies
		for (size_t i = 0; i < s_instances.size(); i++)
		{
			PhysicsBody *body = s_instances[i];
			if (body == this) continue;

			handleInteraction(i, dt);
		}

		glm::vec3 totalVelocity = {};
		for (const glm::vec3 &velocity : m_velocities)
		{
			totalVelocity += velocity;
		}
		m_transform->setLocalTranslation(m_transform->getLocalTranslation() + (totalVelocity * dt));

		Node::update();
	}


	void PhysicsBody::handleInteraction(size_t bodyIndex, float dt)
	{
		PhysicsBody *other = s_instances[bodyIndex];
		glm::vec3 centre = getCentre();
		glm::vec3 otherCentre = other->getCentre();

		// Obtain positive force
		glm::vec3 sep = (otherCentre - centre);
		glm::vec3 force;

		// If separation is approx. 0, then set force to 0.
		if (glm::all(glm::epsilonEqual(sep, {}, Physics::EPSILON)))
			force = {};
		else
			force = glm::normalize(sep) * getGravityScalar(glm::length(sep), other->getMass());

		// Apply F = dp / dt
		glm::vec3 changeInMomentum = force * dt;
		m_velocities[bodyIndex] += (changeInMomentum / getMass());

		// Snap to other centre, if we would pass over it this frame
		if (glm::length(sep) < (glm::length(m_velocities[bodyIndex]) * dt))
		{
			TE_CORE_INFO("HI");

			m_velocities[bodyIndex] = {};
			m_transform->setLocalTranslation(otherCentre);
		}
	}


	float PhysicsBody::getGravityScalar(float distance, float otherMass) const
	{
		// F = Gm1m2/(r*r)
		float f = (Physics::G * getMass() * otherMass) / (distance * distance);
		return f;
	}
}