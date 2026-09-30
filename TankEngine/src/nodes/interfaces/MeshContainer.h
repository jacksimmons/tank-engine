#pragma once
#include "Mesh.h"
#include "ShaderContainer.h"


namespace Tank
{	
	class TANK_API IMeshContainer : public IShaderContainer
	{
		friend class Renderer;
	protected:
		std::unique_ptr<Shader> m_outlineShader;
		bool m_outlineEnabled;
		std::vector<std::unique_ptr<Mesh>> m_meshes;


		IMeshContainer();
	public:
		virtual ~IMeshContainer() = default;
		
		std::vector<Mesh*> getMeshes() const;
		void setOutlineEnabled(bool enabled) noexcept { m_outlineEnabled = enabled; }
	};
}