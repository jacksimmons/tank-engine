#include <shader_source.hpp>
#include <shader.hpp>
#include "shader_container.hpp"


namespace Tank
{
	IShaderContainer::IShaderContainer() = default;
	IShaderContainer::~IShaderContainer() = default;


	void IShaderContainer::initShaderContainer(ShaderSources sources)
	{
		auto maybeShader = Shader::createShader(sources);
		if (maybeShader.has_value())
			m_shader = std::move(maybeShader.value());
		else
			TE_CORE_ERROR("Failed to load shader.");
	}


	void IShaderContainer::setShader(std::unique_ptr<Shader> shader)
	{
		m_shader = std::move(shader);
	}


	const Shader &IShaderContainer::getShader() const
	{
		return *m_shader;
	}
}