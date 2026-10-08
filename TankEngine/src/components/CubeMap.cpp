#include <glad/glad.h>
#include <Log.h>
#include <ShaderSource.h>
#include <Texture.h>
#include "CubeMap.h"
#include "Camera.h"


namespace Tank
{
	ShaderSources CubeMapData::s_defaultSources =
	{
		{
			Res("shaders/skybox.vert", true)
		},

		{
			Res("shaders/skybox.frag", true)
		},

		{}
	};


	CubeMapComponent::CubeMapComponent(const std::array<Resource, 6> &texturePaths)
		: shader({}, CubeMapData::s_defaultSources)
	{
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
		const Shader &shader = shader;

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
}
