#include <algorithm>
#include "log.hpp"
#include "physics_body.hpp"
#include "static/constant.hpp"
#include "static/time.hpp"


namespace Tank
{
	std::vector<PhysicsBody*> PhysicsBody::s_instances;


	PhysicsBody::PhysicsBody(const std::string &name, float mass, const glm::vec3 &velocity)
		: m_mass(mass), m_velocity(velocity), Node(name)
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
		glm::vec3 netForce = {};
		for (PhysicsBody *body : s_instances)
		{
			netForce += getGravityForce(body);
		}

		// Apply F = dp / dt
		glm::vec3 momentum = netForce * Time::getFrameDelta();
		m_velocity += momentum / m_mass;

		// Apply velocity to position: d = v dt
		m_transform->setLocalTranslation(m_transform->getLocalTranslation() + (m_velocity * Time::getFrameDelta()));

		Node::update();
	}


	glm::vec3 PhysicsBody::getGravityForce(PhysicsBody *other) const
	{
		// F = Gm1m2/(r*r)
		glm::vec3 c1 = getCentre();
		glm::vec3 c2 = other->getCentre();
		float r = glm::distance(c1, c2);

		// Handle case where both bodies are in exactly the same position (div by 0)
		if (r == 0) return {};

		float f = (Physics::G * m_mass * other->m_mass) / (r*r);
		
		return (c2 - c1) * f;
	}
}