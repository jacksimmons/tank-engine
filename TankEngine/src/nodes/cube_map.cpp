#include <glad/glad.h>
#include <nodes/cube_map.hpp>
#include <nodes/camera.hpp>
#include <nodes/scene.hpp>
#include <nodes/interfaces/shader_container.hpp>
#include <shader_source.hpp>
#include "log.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "../reflection/node_factory.hpp"


namespace Tank
{
	json CubeMap::serialise()
	{
		json serialised = Node::serialise();

		std::vector<std::string> encodedPaths;
		for (const Res &res : m_texturePaths)
		{
			encodedPaths.push_back(Res::encode(res));
		}
		serialised["cubeMap"] = encodedPaths;

		serialised["shader"] = Shader::serialise(getShader());
		return serialised;
	}


	void CubeMap::deserialise(const json &serialised)
	{
		initShaderContainer(ShaderSources::deserialise(serialised["shader"]));

		std::array<Res, 6> decodedPaths;
		for (int i = 0; i < 6; i++)
		{
			decodedPaths[i] = Res::decode(serialised["cubeMap"][i]);
		}
		setTexPaths(decodedPaths);
		
		Node::deserialise(serialised);
	}


	CubeMap::CubeMap(const std::string &name, const std::array<Resource, 6> &texturePaths)
		: Node(name), IShaderContainer()
	{
		m_type = "CubeMap";
		ShaderSources sources;
		sources.vertex.location = Res("shaders/skybox.vert", true);
		sources.fragment.location = Res("shaders/skybox.frag", true);
		initShaderContainer(sources);

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), &s_vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		setTexPaths(texturePaths);
	}


	void CubeMap::setTexPaths(const std::array<Resource, 6> &texPaths)
	{
		m_texturePaths = texPaths;
		const Shader &shader = getShader();

		shader.use();
		{
			int texNum = Texture::getTexCount();
			auto tex = Texture::cubeMapFromFile(m_texturePaths, "cubeMap");
			if (tex.has_value())
			{
				m_texture = tex.value();
				shader.setInt("cubeMap", 0);
			}
			else
			{
				TE_CORE_ERROR("Failed to create CubeMap.");
			}
		}
		shader.unuse();
	}


	/// <summary>
	/// Must be drawn before anything else in the scene.
	/// </summary>
	void CubeMap::draw()
	{
		const Shader &shader = getShader();
		shader.use();
		
		// Bind all owned texture objects
		int texTarget = m_texture->getTexTarget();
		int texID = m_texture->getTexID();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(texTarget, texID);

		Camera *cam = Scene::getActiveScene()->getActiveCamera();
		shader.setMat4("view", glm::mat4(glm::mat3(cam->getView())));
		shader.setMat4("proj", cam->getProj());

		glDepthMask(GL_FALSE);
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		shader.unuse();

		Node::draw();
	}
}
