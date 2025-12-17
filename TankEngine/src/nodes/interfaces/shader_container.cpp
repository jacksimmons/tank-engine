#include <shader_source.hpp>
#include <shader.hpp>
#include "shader_container.hpp"


namespace Tank
{
	IShaderContainer::IShaderContainer(ShaderSources *pSources)
	{
		initShaderContainer(pSources);
	}


	void IShaderContainer::initShaderContainer(ShaderSources *pSources)
	{
		ShaderSources sources;
		if (pSources)
		{
			sources = *pSources;
		}
		else
		{
			sources.vertex.location = "shader.vert";
			sources.fragment.location = "shader.frag";
		}

		auto maybeShader = Shader::createShader(sources);
		if (maybeShader.has_value())
			m_shader = std::move(maybeShader.value());
		else
			TE_CORE_ERROR("Failed to load shader.");
	}
}