#pragma once
#include "Mesh.h"
#include "ShaderContainer.h"
#include "Outlined.h"


namespace Tank
{
	class IShaderContainer;
	class IOutlined;
	
	class TANK_API IMeshContainer : public IOutlined, public IShaderContainer
	{
	protected:
		std::vector<std::unique_ptr<Mesh>> m_meshes;


		IMeshContainer();
	public:
		virtual ~IMeshContainer() = default;
		
		std::vector<Mesh*> getMeshes() const;
		virtual void drawOutlineMeshes(Shader *outlineShader) override;
	};
}