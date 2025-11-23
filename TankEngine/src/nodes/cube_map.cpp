#include <glad/glad.h>
#include "tepch.hpp"
#include "log.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "nodes/cube_map.hpp"
#include "nodes/camera.hpp"
#include "nodes/scene.hpp"
#include "../reflection/node_factory.hpp"


namespace Tank
{
	json CubeMap::serialise()
	{
		json serialised = Node::serialise();
		serialised["cubeMap"] = m_texturePaths;
		serialised["shader"] = Shader::serialise(*(m_shader));
		return serialised;
	}


	void CubeMap::deserialise(const json &serialised, CubeMap **targetPtr)
	{
		ShaderSources sources;
		sources.vertex.location = std::string{ serialised["shader"]["vert"] };
		sources.fragment.location = std::string{ serialised["shader"]["frag"] };
		sources.geometry.location = std::string{ serialised["shader"]["geom"] };
		if (!(*targetPtr)) *targetPtr = new CubeMap(serialised["name"], sources, serialised["cubeMap"]);

		Node *target = *targetPtr;
		target->deserialise(serialised);
	}


	CubeMap::CubeMap(const std::string &name, ShaderSources &sources, const std::array<std::string, 6> &textureNames)
		: Node(name), IShaderContainer(sources)
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

		m_shader->use();
		{
			int texNum = Texture::getTexCount();
			auto tex = Texture::cubeMapFromFile(fs::current_path() / "textures", textureNames, "cubeMap");
			if (tex.has_value())
			{
				m_texture = tex.value();
				m_shader->setInt("cubeMap", texNum);
				TE_CORE_INFO(std::format("Set cubeMap to {}, first filename {}", texNum, textureNames[0]));
			}
			else
			{
				TE_CORE_ERROR(std::format("GL_TEXTURE_CUBE_MAP Tex number {} uniform was not set.", texNum));
			}
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
