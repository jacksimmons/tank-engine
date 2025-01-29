#include <glad/glad.h>
#include <nodes/model.hpp>
#include "cube.hpp"
#include "log.hpp"


namespace Tank
{
	Cube::Cube(const std::string &name, const std::string &vsName, const std::string &fsName)
		: Model(name, vsName, fsName, (float *)&Cube::s_vertices, sizeof(Cube::s_vertices))
	{
		if (!addTexture("awesomeface.png", GL_RGBA, "material.diffuse"))
			TE_CORE_ERROR("Failed to load diffuse texture.");
		if (!addTexture("specular.png", GL_RGBA, "material.specular"))
			TE_CORE_ERROR("Failed to load specular texture.");
	}


	void Cube::draw()
	{
		m_shader->use();
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		m_shader->unuse();

		Model::draw();
	}
}