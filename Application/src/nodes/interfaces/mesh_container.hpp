#pragma once

#include "mesh.hpp"
#include "shader_container.hpp"


namespace Tank
{
	class IMeshContainer : public IShaderContainer
	{
	protected:
		std::vector<std::unique_ptr<Mesh>> m_meshes;
		IMeshContainer(const std::string &name, ShaderSources &sources)
			: IShaderContainer(name, sources)
		{
		}
	public:
		virtual ~IMeshContainer() = default;

		
		std::vector<Mesh*> getMeshes() const;
		virtual void drawOutlineMeshes(Shader *outlineShader) override;
	};
}