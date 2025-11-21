#pragma once
#include <format>
#include "log.hpp"


namespace Tank
{
	class TANK_API Texture
	{
	private:
		static unsigned s_numTextures;
		unsigned m_texID; // OpenGL's internal value for this texture
		unsigned m_texTarget; // GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP
		std::string m_texType; // material.{diffuse | specular}
		fs::path m_path;

		// No reason to expose this constructor. Textures must be built using the factories below
		Texture(unsigned texID, unsigned texTarget, const std::string &uniformName, const fs::path &path);
	public:
		~Texture();
		unsigned getTexID() const noexcept { return m_texID; };
		unsigned getTexTarget() const noexcept { return m_texTarget; }
		const std::string &getTexType() const { return m_texType; }
		const fs::path &getPath() const { return m_path; }
		static unsigned getTexCount() { return s_numTextures; };

		static void initSTBI();
		static std::optional<std::shared_ptr<Texture>> fromFile(const fs::path &path, const std::string &texType);
		static std::optional<std::shared_ptr<Texture>> cubeMapFromFile(const fs::path &directory, const std::array<std::string, 6> &filenames,
			const std::string &texType);
	};
}