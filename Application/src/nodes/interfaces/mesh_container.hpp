#pragma once

#include "mesh.hpp"
#include "shader_container.hpp"


namespace Tank
{
	class IMeshContainer : public IShaderContainer
	{
	protected:
		std::vector<Mesh> m_meshes;
		IMeshContainer(const std::string &name, ShaderSources &sources)
			: IShaderContainer(name, sources)
		{
		}
	public:
		virtual ~IMeshContainer() = default;

		
		const std::vector<Mesh> &getMeshes() const { return m_meshes; }
		virtual void drawOutlineMeshes(Shader *outlineShader) override;
	};
}