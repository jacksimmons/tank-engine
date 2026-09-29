#include <glm/gtx/quaternion.hpp>
#include <static/GlmSerialise.h>
#include "Transform.h"


namespace Tank
{
	glm::mat4 TransformComponent::getLocalModelMatrix() const
	{
		glm::mat4 model = glm::identity<glm::mat4>();

		model = glm::translate(model, m_translation);
		model = glm::mat4_cast(m_rotation) * model;
		model = glm::scale(model, m_scale);

		return model;
	}
}