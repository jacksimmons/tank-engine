#pragma once


namespace Tank
{
	/// <summary>
	/// Base class for a Node which interacts with the physics engine.
	/// Has a mass, and exerts gravity on all bodies in the scene.
	/// </summary>
	struct PhysicsBodyComponent
	{
	private:
		static std::vector<PhysicsBodyComponent*> s_instances;
		std::vector<glm::vec3> m_velocities;

		void handleInteraction(size_t bodyIndex, float dt);
		float getGravityScalar(float distance, float otherMass) const;
	public:
		float mass;

		PhysicsBodyComponent(float mass = 1);
		virtual ~PhysicsBodyComponent();

		glm::vec3 getCentre() const noexcept;
	};
}
