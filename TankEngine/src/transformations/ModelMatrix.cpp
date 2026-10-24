#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include <components/Transform.h>
#include "ModelMatrix.hpp"


namespace Tank
{
	glm::mat4 ModelMatrix::world(const TransformComponent &transform)
	{
		// If this transform has a parent, pre-multiply by ModelMatrix::world(parent).
		auto parent = transform.getParent();
		if (parent != nullptr)
		{
			return ModelMatrix::world(parent->getComponent<TransformComponent>()) * (glm::mat4)transform;
		}

		// Otherwise, this transform is a root, return operator().
		return (glm::mat4)transform;
	}
}