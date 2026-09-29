#pragma once


namespace Tank
{
	struct SpriteComponent;
	class Shader;
	class Mesh;


	class Renderer2D
	{
	public:
		static void drawSprite(const glm::mat4 &transform, SpriteComponent &sprite);
		static void drawMesh(const Shader &shader, const Mesh &mesh);
	};
}