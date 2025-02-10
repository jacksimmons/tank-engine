#include <string>
#include <array>
#include <memory>
#include <glad/glad.h>
#include "cube_map.hpp"
#include "scene.hpp"
#include "texture.hpp"
#include "log.hpp"
#include "nodes/camera.hpp"


namespace Tank
{
	json CubeMap::serialise(CubeMap *cubeMap)
	{
		json serialised = Node::serialise(cubeMap);

		serialised["vsPath"] = cubeMap->m_shader->getVertPath();
		serialised["fsPath"] = cubeMap->m_shader->getFragPath();
		serialised["texturePaths"] = cubeMap->m_texturePaths;

		return serialised;
	}


	void CubeMap::deserialise(const json &serialised, CubeMap **targetPtr)
	{
		if (!(*targetPtr)) *targetPtr = new CubeMap(serialised["name"], serialised["vsPath"], serialised["fsPath"], serialised["texturePaths"]);

		Node *target = *targetPtr;
		Node::deserialise(serialised, &target);
	}


	CubeMap::CubeMap(const std::string &name, const std::string &vsName, const std::string &fsName, const std::array<std::string, 6> &textureNames)
		: Node(name)
	{
		m_type = "CubeMap";
		m_texturePaths = textureNames;

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), &s_vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		m_shader = std::make_unique<Shader>(vsName, fsName);
		m_shader->use();

		int texNum = Texture::getTexCount();
		auto tex = Texture::cubeMapFromFile(std::string(ROOT_DIRECTORY) + "/textures/skybox", textureNames, "cubeMap");
		if (tex.has_value())
		{
			m_texture = tex.value();
			m_shader->setInt("cubeMap", texNum);
			TE_CORE_INFO(std::format("Set {} to {}, first filename {}", "cubeMap", texNum, textureNames[0]));
		}
		else
		{
			TE_CORE_ERROR(std::format("GL_TEXTURE_CUBE_MAP GL_TEXTURE{} uniform was not set.", texNum));
		}
		m_shader->unuse();
	}


	/// <summary>
	/// Must be drawn before anything else in the scene.
	/// </summary>
	void CubeMap::draw()
	{
		m_shader->use();
		
		// Bind all owned texture objects
		int texTarget = m_texture->getTexTarget();
		int texID = m_texture->getTexID();
		glActiveTexture(GL_TEXTURE0);
		m_shader->setInt("cubeMap", 0);
		glBindTexture(texTarget, texID);

		Camera *cam = Scene::getActiveScene()->getActiveCamera();
		m_shader->setMat4("view", glm::mat4(glm::mat3(cam->getView())));
		m_shader->setMat4("proj", cam->getProj());

		glDepthMask(GL_FALSE);
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		m_shader->unuse();

		Node::draw();
	}
}