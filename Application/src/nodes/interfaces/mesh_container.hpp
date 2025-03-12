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

		virtual void drawOutlineMeshes(Shader *outlineShader) override
		{
			for (unsigned i = 0; i < m_meshes.size(); i++)
			{ // use
				m_meshes[i].draw(outlineShader);
			} // unuse
		}
	};
}