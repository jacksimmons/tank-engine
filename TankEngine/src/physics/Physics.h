#pragma once


namespace Tank
{
	struct PhysicsBodyComponent;
	namespace entt { typedef unsigned entity; }


	namespace PHYSICS
	{
		const double G = 6.6743e-11;
		const float EPSILON = 1e-5;
	}


	class Physics
	{
		static std::unordered_map<entt::entity, glm::vec3> s_velocities;

	public:
		static float gravitation(float M, float m, float r);
		/// @brief Applies gravititation acceleration to m due to M, over a timestep of dt.
		/// Stores the resulting velocity from this acceleration in s_velocities.
		static void handleGravity(entt::entity e, const PhysicsBodyComponent &M, const PhysicsBodyComponent &m, float dt);
	};
}