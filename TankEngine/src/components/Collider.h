#pragma once
#include <shapes/CollisionShape.h>


namespace Tank
{
	struct ColliderComponent
	{
		std::unique_ptr<CollisionShape> Shape;

		ColliderComponent() = default;
		ColliderComponent(std::unique_ptr<CollisionShape> shape) : Shape(std::move(shape)) {};
	};
}
