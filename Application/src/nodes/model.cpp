#include <iostream>
#include <filesystem>
#include <array>
#include <memory>
#include <algorithm>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "log.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "scene.hpp"
#include "transform.hpp"
#include "nodes/model.hpp"
#include "nodes/camera.hpp"
#include "nodes/light.hpp"


namespace Tank
{
	Model::Model(const std::string &name, const std::string &vsName, const std::string &fsName, float *vertices, size_t verticesSize)
		: TexturedNode(name, vsName, fsName)
	{
		// ===== INIT VBO/VAO =====
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//// Colour
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
		//glEnableVertexAttribArray(1);

		// Texture coords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}


	void Model::draw()
	{
		for (int i = 0; i < m_textures.size(); i++)
		{
			glActiveTexture(m_textures[i]->getTexPos());
			glBindTexture(GL_TEXTURE_2D, m_textures[i]->getTexID());
		}

		m_shader->use();

		auto cam = Scene::getActiveScene()->getActiveCamera();
		auto P = cam->getProj();
		auto V = cam->getView();
		auto M = getTransform()->getWorldModelMatrix();
		auto VM = V * M;
		m_shader->setMat4("PVM", P * VM);
		m_shader->setMat4("VM", VM);
		m_shader->setMat4("V", V);
		m_shader->setMat4("VM_it", glm::inverseTranspose(VM));
		m_shader->setVec3("tex_scale", glm::vec3{ 1, 1, 1 });
		m_shader->setFloat("material.Ns", 32.0f);

		for (Light *light : Scene::getActiveScene()->getActiveLights())
		{
			light->updateShader(m_shader.get());
		}
		m_shader->setInt("num_dir_lights", DirLight::getCount());
		m_shader->setInt("num_point_lights", PointLight::getCount());

		m_shader->unuse();

		TexturedNode::draw();
	}


	void Model::update()
	{
		Node::update();
	}
}