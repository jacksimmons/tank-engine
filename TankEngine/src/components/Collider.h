#pragma once
#include <shapes/CollisionShape.h>


namespace Tank
{
	struct ColliderComponent
	{
		std::unique_ptr<CollisionShape> shape;

		ColliderComponent() = default;
		ColliderComponent(std::unique_ptr<CollisionShape> shape) : shape(std::move(shape)) {};
	};
}
