#include <glad/glad.h>
#include "outlined.hpp"
#include "nodes/scene.hpp"
#include "nodes/camera.hpp"
#include "shader.hpp"
#include "mesh.hpp"


namespace Tank
{
	IOutlined::IOutlined(const glm::vec4 &outlineCol)
	{
		ShaderSources sources;
		sources.vertex.location = "shader.vert";
		sources.fragment.location = "outline/single_colour.frag";
		auto shader = Shader::createShader(sources);

		if (shader.has_value())
		{
			m_outlineShader = std::move(shader.value());
		}
		else
		{
			TE_CORE_ERROR("IOutlined: Invalid shader.");
		}

		m_outlineShader->use();
		m_outlineShader->setVec4("outline_col", outlineCol);
		m_outlineShader->unuse();
		m_outlineEnabled = false;
	}


	/// <summary>
	/// Sets up the stencil buffer to write all fragments rendered
	/// after this is called (to the stencil buffer).
	/// 
	/// Make sure to render (draw) the object after this is called.
	/// </summary>
	void IOutlined::predraw()
	{
		if (!m_outlineEnabled) return;

		// Pass or discard? All frags pass, as GL_ALWAYS is used.
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		// Each bit is written to the stencil buffer as is.
		glStencilMask(0xFF);
	}


	/// <summary>
	/// After object is drawn, disable stencil writing and depth
	/// testing. Then draw a scaled-up version of the object, in
	/// a block colour.
	/// </summary>
	void IOutlined::postdraw(Transform *transform)
	{
		if (!m_outlineEnabled) return;

		// Only draw parts of the object that are outside of the drawn
		// shape. If concealed, this will include the entire drawn shape.
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		// Disable stencil writing and depth testing
		glStencilMask(0x00);

		m_outlineShader->use(); // use

		// Setup uniforms
		Camera *cam = Scene::getActiveScene()->getActiveCamera();
		const glm::vec3 scale = transform->getLocalScale();
		const glm::vec3 trans = transform->getLocalTranslation();
		transform->setLocalScale(scale * 1.025f);
		m_outlineShader->setMat4("PVM", cam->getProj() * cam->getView() * transform->getWorldModelMatrix());
		transform->setLocalScale(scale);

		drawOutlineMeshes(m_outlineShader.get());

		// Now disable writing to the stencil buffer.
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
	}
}