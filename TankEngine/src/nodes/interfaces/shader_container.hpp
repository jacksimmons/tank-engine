#pragma once
#include <utils/getset.hpp>



namespace Tank
{
	class Texture;
	struct ShaderSources;
	class Shader;

	/// <summary>
	/// An interface for an object containing (currently one) shaders.
	/// </summary>
	class TANK_API IShaderContainer
	{
	private:
		std::unique_ptr<Shader> m_shader;
	protected:
		void initShaderContainer(ShaderSources *sources);
	public:

		IShaderContainer(ShaderSources *sources);
		virtual ~IShaderContainer() = default;


		void setShader(std::unique_ptr<Shader> shader) { m_shader = std::move(shader); };
		const Shader &getShader() const { return *m_shader; }
	};
}