#include <ShaderSource.h>
#include "MeshContainer.h"


namespace Tank
{
	IMeshContainer::IMeshContainer()
		: IOutlined(), IShaderContainer()
	{
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


	void IMeshContainer::drawOutlineMeshes(Shader *outlineShader)
	{
		for (unsigned i = 0; i < m_meshes.size(); i++)
		{ // use
			m_meshes[i]->draw(*outlineShader);
		} // unuse
	}
}