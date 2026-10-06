#include <glad/glad.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <scene/Scene.h>
#include <scene/Camera.h>
#include <components/Sprite.h>
#include <components/Light.h>
#include <Texture.h>
#include <Shader.h>
#include <Mesh.h>
#include "Renderer2D.h"
#include "Renderer.h"


namespace Tank
{
	void Renderer2D::drawSprite(TransformComponent &transform, const SpriteComponent &sprite, const Camera &camera)
	{
		Renderer::beginEditorOutline(sprite);

		const Shader &shader = sprite.shader;
		shader.use();

		shader.setVec3("tex_scale", glm::vec3{ 1, 1, 1 });
		shader.setFloat("material.Ns", 32.0f);

		auto cam = Scene::getActiveScene()->getActiveCamera();
		auto P = camera.m_projection;
		auto V = camera.m_view;
		auto M = transform.getWorldModelMatrix();
		auto VM = V * M;

		shader.setMat4("PVM", P * VM);
		shader.setMat4("VM", VM);
		shader.setMat4("V", V);
		shader.setMat4("VM_it", glm::inverseTranspose(VM));

		for (unsigned i = 0; i < sprite.getMeshes().size(); i++)
		{
			Renderer::drawMesh(shader, *sprite.getMeshes()[i]);
		}

		shader.unuse();

		Renderer::endEditorOutline(transform, sprite, camera);
	}
}