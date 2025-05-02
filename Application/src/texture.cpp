#include <glad/glad.h>
#include "texture.hpp"
#include "nodes/model.hpp"
namespace fs = std::filesystem;


namespace Tank
{
	unsigned Texture::s_numTextures = 0;


	Texture::Texture(GLuint texID, GLenum texTarget, const std::string &texType, const std::string &directory, const std::string &filename) :
		m_texID(texID), m_texTarget(texTarget), m_texType(texType), m_directory(directory), m_filename(filename)
	{
		s_numTextures++;
	}


	Texture::~Texture()
	{
		GLuint id = getTexID();
		glDeleteTextures(1, &id);

		s_numTextures--;
	}


	std::optional<std::shared_ptr<Texture>> Texture::fromFile(const std::string &directory, const std::string &filename, const std::string &texType)
	{
		//GLint maxTextureUnits;
		//glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
		//if (s_numTextures >= (GLuint)maxTextureUnits)
		//{
		//	TE_CORE_ERROR("Max texture units surpassed.");
		//	return {};
		//}

		int w, h, numChannels;
		unsigned char *data = stbi_load((directory + "/" + filename).c_str(), &w, &h, &numChannels, 0); // +stbi1

		if (!data)
		{
			TE_CORE_ERROR("Failed to load texture from image. File:");
			TE_CORE_ERROR(std::string(directory) + "/" + filename);
			stbi_image_free(data); // -stbi1
			return {};
		}

		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		GLenum mode = GL_RGBA;
		if (numChannels == 1) mode = GL_RED;
		else if (numChannels == 3) mode = GL_RGB;
		else if (numChannels == 4) mode = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data); // -stbi1
		glGenerateMipmap(GL_TEXTURE_2D);

		// Textures
		float borderColour[] = { 0.0f, 1.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		TE_CORE_INFO(std::format("Added GL_TEXTURE_2D texture {} with ID {}.", filename, texID));

		return std::shared_ptr<Texture>(new Texture(texID, GL_TEXTURE_2D, texType, directory, filename));
	}


	std::optional<std::shared_ptr<Texture>> Texture::cubeMapFromFile(const std::string &directory, const std::array<std::string, 6> &filenames,
		const std::string &texType)
	{
		//GLint maxTextureUnits;
		//glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
		//if (s_numTextures >= (GLuint)maxTextureUnits) return {};

		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
		for (int i = 0; i < 6; i++)
		{
			int w, h, numChannels;
			unsigned char *data = stbi_load((directory + "/" + filenames[i]).c_str(), &w, &h, &numChannels, 0); // +stbi1

			if (!data)
			{
				TE_CORE_ERROR("Failed to load texture from image. File:");
				TE_CORE_ERROR(filenames[i]);
				stbi_image_free(data); // -stbi1
				return {};
			}


			GLenum mode = GL_RGBA;
			if (numChannels == 1) mode = GL_RED;
			else if (numChannels == 3) mode = GL_RGB;
			else if (numChannels == 4) mode = GL_RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, data);

			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, w, h, 0, mode, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data); // -stbi1
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		TE_CORE_INFO(std::format("Added GL_TEXTURE_CUBE_MAP texture (first: {}/{}) with name {}", directory, filenames[0], texID));

		return std::shared_ptr<Texture>(new Texture(texID, GL_TEXTURE_CUBE_MAP, texType, directory, filenames[0]));
	}
}