#include "shader_container.hpp"


namespace Tank
{
	std::vector<std::weak_ptr<Texture>> IShaderContainer::s_loadedTextures;


	IShaderContainer::IShaderContainer(
		const std::string &name,
		ShaderSources &sources) : IOutlined(name)
	{
		auto maybeShader = Shader::createShader(sources);
		if (maybeShader.has_value())
			m_shader = std::move(maybeShader.value());
	}


	void IShaderContainer::touchLoadedTextures()
	{
		std::vector<size_t> toRemoveIndices;

		// Vector resizing occurs in this loop; continuously check we are in vector bounds.
		size_t index = 0;
		while (index < s_loadedTextures.size())
		{
			if (s_loadedTextures[index].expired()) s_loadedTextures.erase(s_loadedTextures.begin() + index);
			else index++;
		}
	}


	void IShaderContainer::draw()
	{
	}


	std::vector<std::shared_ptr<Texture>> IShaderContainer::getLoadedTextures()
	{
		touchLoadedTextures();

		std::vector<std::shared_ptr<Texture>> textures;
		textures.resize(s_loadedTextures.size());
		std::transform(
			s_loadedTextures.begin(),
			s_loadedTextures.end(),
			textures.begin(),
			[](std::weak_ptr<Texture> tex) { return tex.lock(); }
		);
		return textures;
	}

	
	void IShaderContainer::addLoadedTexture(std::weak_ptr<Texture> texture)
	{
		s_loadedTextures.push_back(texture);
	}
}