#include <glad/glad.h>
#include <nodes/interfaces/ShaderContainer.h>
#include <reflection/NodeFactory.h>
#include <Log.h>
#include <Shader.h>
#include <Texture.h>
#include "CubeMap.h"
#include "Camera.h"
#include "Scene.h"


namespace Tank
{
	CubeMapComponent::CubeMapComponent(const std::array<Resource, 6> &texturePaths)
		: IShaderContainer()
	{
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

		setTexPaths(texturePaths);
	}


	void CubeMapComponent::setTexPaths(const std::array<Resource, 6> &texPaths)
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
	void CubeMapComponent::draw()
	{
		const Shader &shader = getShader();
		shader.use();

		// Bind all owned texture objects
		int texTarget = m_texture->getTexTarget();
		int texID = m_texture->getTexID();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(texTarget, texID);

		CameraComponent *cam = Scene::getActiveScene()->getActiveCamera();
		shader.setMat4("view", glm::mat4(glm::mat3(cam->View)));
		shader.setMat4("proj", cam->Projection);

		glDepthMask(GL_FALSE);
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		shader.unuse();

	}


	// =======================
	//		Serialisation
	// =======================
	template <>
	json serialise<CubeMapComponent>(CubeMapComponent *in)
	{
		json serialised;

		std::vector<std::string> encodedPaths;
		for (const Res &res : in->m_texturePaths)
		{
			encodedPaths.push_back(Res::encode(res));
		}
		serialised["cubeMap"] = encodedPaths;

		serialised["shader"] = Shader::serialise(in->getShader());
		return serialised;
	}

	template <>
	void deserialise<CubeMapComponent>(const json &serialised, CubeMapComponent *out)
	{
		out->initShaderContainer(ShaderSources::deserialise(serialised["shader"]));

		std::array<Res, 6> decodedPaths;
		for (int i = 0; i < 6; i++)
		{
			decodedPaths[i] = Res::decode(serialised["cubeMap"][i]);
		}
		out->setTexPaths(decodedPaths);
	}
}
