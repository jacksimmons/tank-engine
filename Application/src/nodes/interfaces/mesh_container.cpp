#include "mesh_container.hpp"


namespace Tank
{
	void IMeshContainer::drawOutlineMeshes(Shader *outlineShader)
	{
		for (unsigned i = 0; i < m_meshes.size(); i++)
		{ // use
			m_meshes[i].draw(outlineShader);
		} // unuse
	}
}