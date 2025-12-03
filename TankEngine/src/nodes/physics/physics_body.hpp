#pragma once
#include "transformation.hpp"
#include "nodes/node.hpp"
#include "interfaces/mass.hpp"


namespace Tank
{
	/// <summary>
	/// Base class for a Node which interacts with the physics engine.
	/// Has a mass, and exerts gravity on all bodies in the scene.
	/// </summary>
	class TANK_API PhysicsBody : public Node, public IMass
	{
	private:
		static std::vector<PhysicsBody*> s_instances;
		std::vector<glm::vec3> m_velocities;

		void handleInteraction(size_t bodyIndex, float dt);
		float getGravityScalar(float distance, float otherMass) const;
	public:
		PhysicsBody(const std::string &name = "PhysicsBody", float mass = 1);
		virtual ~PhysicsBody();

		glm::vec3 getCentre() const noexcept { return mat4::getTranslation(m_transform->getWorldModelMatrix()); }

		void update() override;
	};
}