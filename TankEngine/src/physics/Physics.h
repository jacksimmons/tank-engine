#pragma once


namespace Tank
{
	struct PhysicsBodyComponent;


	namespace PHYSICS
	{
		const double G = 6.6743e-11;
		const float EPSILON = 1e-5;
	}


	class Physics
	{
	public:
		static float gravitation(float M, float m, float r);
		static void handleGravity(const PhysicsBodyComponent &M, const PhysicsBodyComponent &m, float dt);
	};
}