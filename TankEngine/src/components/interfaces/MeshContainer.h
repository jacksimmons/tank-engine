#pragma once
#include "Mesh.h"


namespace Tank
{	
	class Shader;


	class TANK_API IMeshContainer
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