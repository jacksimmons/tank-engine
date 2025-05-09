#pragma once

#include "shader.hpp"
#include "transform.hpp"


namespace Tank
{
	class Mesh; class Transform;
	class IOutlined
	{
	private:
		std::unique_ptr<Shader> m_outlineShader;
		bool m_outlineEnabled;
	protected:
		IOutlined(const glm::vec4 &outlineCol = { 0.5f, 0, 0, 1 });
	public:
		virtual ~IOutlined() = default;

		void setOutlineEnabled(bool enabled) noexcept { m_outlineEnabled = enabled; }
		void predraw();
		void postdraw(Transform *transform);

		/// <summary>
		/// Called as part of postdraw.
		/// 
		/// Draw all the meshes in the object, with outline shader enabled and transform
		/// temporarily scaled up.
		/// </summary>
		virtual void drawOutlineMeshes(Shader *outlineShader) = 0;
	};
}