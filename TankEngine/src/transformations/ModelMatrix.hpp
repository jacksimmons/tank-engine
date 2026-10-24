#pragma once


namespace Tank
{
	struct TransformComponent;

	class TANK_API ModelMatrix
	{
	public:
		/// @brief The world model matrix, calculated by a transform recursively pre-multiplied by each ancestor.
		static glm::mat4 world(const TransformComponent &transform);
	};
}