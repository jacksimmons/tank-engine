#pragma once


namespace Tank
{
	struct TransformComponent;
	struct SpriteComponent;
	class Shader;
	class Mesh;
	class SceneCamera;


	class Renderer2D
	{
	public:
		static void drawSprite(TransformComponent &transform, const SpriteComponent &sprite, const SceneCamera &camera);
	};
}