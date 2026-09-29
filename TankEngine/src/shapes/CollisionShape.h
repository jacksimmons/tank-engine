#pragma once


namespace Tank
{
	class CollisionShape
	{
	public:
		CollisionShape() = default;
		virtual ~CollisionShape() = default;

		virtual bool contains(const glm::vec3 &point) = 0;
	};
}