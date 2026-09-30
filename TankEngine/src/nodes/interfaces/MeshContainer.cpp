#include <glad/glad.h>
#include <scene/Scene.h>
#include <components/Camera.h>
#include <Shader.h>
#include <Mesh.h>
#include <ShaderSource.h>
#include "MeshContainer.h"


namespace Tank
{
	IMeshContainer::IMeshContainer()
		: IShaderContainer()
	{
		const glm::vec4 &outlineCol = { 0.5f, 0, 0, 1 };

		ShaderSources sources;
		sources.vertex.location = Res("shaders/shader.vert", true);
		sources.fragment.location = Res("shaders/outline/single_colour.frag", true);
		auto shader = Shader::createShader(sources);

		if (shader.has_value())
		{
			m_outlineShader = std::move(shader.value());
		}
		else
		{
			TE_CORE_CRITICAL("IMeshContainer: Invalid shader.");
		}

		m_outlineShader->use();
		m_outlineShader->setVec4("outline_col", outlineCol);
		m_outlineShader->unuse();
		m_outlineEnabled = false;

		ShaderSources sources;
		initShaderContainer(sources);
	}


	std::vector<Mesh*> IMeshContainer::getMeshes() const
	{
		std::vector<Mesh*> meshes;
		for (const std::unique_ptr<Mesh> &mesh : m_meshes)
		{
			meshes.push_back(mesh.get());
		}
		return meshes;
	}
}