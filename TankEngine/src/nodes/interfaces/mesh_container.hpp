#pragma once
#include "mesh.hpp"
#include "shader_container.hpp"
#include "outlined.hpp"


namespace Tank
{
	class IShaderContainer;
	class IOutlined;
	
	class TANK_API IMeshContainer : public IOutlined, public IShaderContainer
	{
	protected:
		std::vector<std::unique_ptr<Mesh>> m_meshes;


		IMeshContainer(ShaderSources *sources) : IOutlined(), IShaderContainer(sources)
		{
		}
	public:
		virtual ~IMeshContainer() = default;
		
		std::vector<Mesh*> getMeshes() const;
		virtual void drawOutlineMeshes(Shader *outlineShader) override;
	};
}