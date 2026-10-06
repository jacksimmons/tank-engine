#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/epsilon.hpp>
#include <Log.h>
#include <static/Math.h>
#include <static/Time.h>
#include "PhysicsBody.h"


namespace Tank
{
	std::vector<PhysicsBodyComponent*> PhysicsBodyComponent::s_instances;


	PhysicsBodyComponent::PhysicsBodyComponent(float mass) : mass(mass)
	{
		s_instances.push_back(this);
	}


	PhysicsBodyComponent::~PhysicsBodyComponent()
	{
		auto it = std::find(s_instances.begin(), s_instances.end(), this);
		if (it != s_instances.end())
			s_instances.erase(it);
		else
			TE_CORE_FATAL("Destructor: PhysicsBody was missing from s_instances.");
	}


	glm::vec3 PhysicsBodyComponent::getCentre() const noexcept
	{
		return glm::vec3(0.0f);
	}
}
