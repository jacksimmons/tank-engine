#pragma once
#include "mesh.hpp"
#include "shader_container.hpp"
#include "outlined.hpp"


namespace Tank
{
	class IShaderContainer; class IOutlined;
	class IMeshContainer : public IShaderContainer, public IOutlined
	{
	protected:
		std::vector<std::unique_ptr<Mesh>> m_meshes;
		IMeshContainer(ShaderSources &sources) : IShaderContainer(sources), IOutlined()
		{
		}
	public:
		virtual ~IMeshContainer() = default;

		
		std::vector<Mesh*> getMeshes() const;
		virtual void drawOutlineMeshes(Shader *outlineShader) override;
	};
}