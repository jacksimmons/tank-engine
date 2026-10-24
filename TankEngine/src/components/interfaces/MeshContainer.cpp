#include <glad/glad.h>
#include <scene/Scene.h>
#include <components/Camera.h>
#include <Shader.h>
#include <Mesh.h>
#include <ShaderSource.h>
#include <utils/Vector.h>
#include "MeshContainer.h"


namespace Tank
{
	IMeshContainer::IMeshContainer()
	{
		const glm::vec4 &outlineCol = { 0.5f, 0, 0, 1 };

		ShaderSources sources;
		sources.vertex.location = Res("shaders/shader.vert", true);
		sources.fragment.location = Res("shaders/outline/single_colour.frag", true);
		
		m_outlineShader = std::make_unique<Shader>(std::make_optional<unsigned>(), sources);
		m_outlineShader->use();
		m_outlineShader->setVec4("outline_col", outlineCol);
		m_outlineShader->unuse();
		m_outlineEnabled = false;
	}


	std::vector<Mesh*> IMeshContainer::getMeshes() const
	{
		return Vector::UniquePtr::toRawPtr(m_meshes);
	}
}