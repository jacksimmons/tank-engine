#pragma once

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
		// Non-owning reference to all loaded textures. Once all owners of a texture are
		// destroyed, the corresponding entry in this vector becomes invalid.
		static std::vector<std::weak_ptr<Texture>> s_loadedTextures;
		std::unique_ptr<Shader> m_shader;

		/// <summary>
		/// Checks all entries in loadedTextures for nullptr references, and removes any.
		/// Only needed when retrieving data (getLoadedTextures).
		/// </summary>
		static void touchLoadedTextures();

	protected:
		void initShaderContainer(ShaderSources *sources);
	public:
		IShaderContainer(ShaderSources *sources);
		virtual ~IShaderContainer() = default;


		void setShader(std::unique_ptr<Shader> shader) { m_shader = std::move(shader); };
		const Shader &getShader() const { return *m_shader; }

		static std::vector<std::shared_ptr<Texture>> getLoadedTextures();
		static void addLoadedTexture(std::weak_ptr<Texture> texture);
	};
}