#include <glad/glad.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <nodes/interfaces/Outlined.h>
#include <scene/Scene.h>
#include <components/Sprite.h>
#include <components/Camera.h>
#include <components/Light.h>
#include <Texture.h>
#include <Shader.h>
#include <Mesh.h>
#include "Renderer2D.h"


namespace Tank
{
	void Renderer2D::drawSprite(const glm::mat4 &modelMatrix, SpriteComponent &sprite)
	{
		sprite.predraw();

		const Shader &shader = sprite.getShader();
		shader.use();

		shader.setVec3("tex_scale", glm::vec3{ 1, 1, 1 });
		shader.setFloat("material.Ns", 32.0f);

		TransformComponent transform;
		auto cam = Scene::getActiveScene()->getActiveCamera();
		auto P = cam->Camera.m_projection;
		auto V = cam->Camera.m_view;
		auto M = modelMatrix;
		auto VM = V * M;

		shader.setMat4("PVM", P * VM);
		shader.setMat4("VM", VM);
		shader.setMat4("V", V);
		shader.setMat4("VM_it", glm::inverseTranspose(VM));

		auto scene = Scene::getActiveScene();
		auto activeLights = scene->getLights();
		for (LightComponent *light : activeLights)
		{
			light->updateShader(shader);
		}

		shader.setInt("num_dir_lights", scene->getNumLights(LightType::Directional));
		shader.setInt("num_point_lights", scene->getNumLights(LightType::Point));

		for (unsigned i = 0; i < sprite.getMeshes().size(); i++)
		{
			drawMesh(shader, *sprite.getMeshes()[i]);
		}

		shader.unuse();

		sprite.postdraw(transform);
	}


	void Renderer2D::drawMesh(const Shader &shader, const Mesh &mesh)
	{
		unsigned int diffuseIdx = 0;
		unsigned int specularIdx = 0;

		shader.use();

		for (unsigned int i = 0; i < mesh.m_textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::string name = mesh.m_textures[i]->getTexType();
			std::string number;

			if (name == "diffuse")
				number = std::to_string(diffuseIdx++);
			else if (name == "specular")
				number = std::to_string(specularIdx++);

			shader.setInt("material." + name + "[" + number + "]", i);

			glBindTexture(GL_TEXTURE_2D, mesh.m_textures[i]->getTexID());
		}

		// Draw mesh vertices
		glBindVertexArray(mesh.m_vao);
		glDrawElements(GL_TRIANGLES, mesh.m_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		shader.unuse();
	}
}