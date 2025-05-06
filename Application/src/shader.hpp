#pragma once
#include <optional>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "log.hpp"
#include "file.hpp"
#include "shader_source.hpp"
namespace fs = std::filesystem;


namespace Tank
{
	/// <summary>
	/// A Shader represents a shader program in OpenGL.
	/// Consists of all parts of the rendering pipeline.
	/// </summary>
	class Shader
	{
	public:
		static json serialise(const Shader &shader);
		static Shader *deserialise(const json &serialised);
	private:
		unsigned m_id;
		ShaderSources m_sources;

		int getLoc(const std::string &name) const;

		Shader(unsigned progId, const ShaderSources &sources);
	public:
		~Shader();


		static std::optional<std::unique_ptr<Shader>> createShader(ShaderSources &shaders);
		static bool attachShader(unsigned programID, ShaderSource &source);
		static bool readShaderFile(const fs::path &shaderPath, std::string &shaderContents, const std::string &shaderType);
		static std::optional<unsigned> compileShader(const std::string &shaderContents, unsigned shaderType, const std::string &shaderTypeStr);


		void use() const;
		void unuse() const;
		unsigned getID() const noexcept { return m_id; };

		const ShaderSources &getShaderSources() const noexcept { return m_sources; }

		void setInt(const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;
		void setVec3(const std::string &name, const glm::vec3 &value) const;
		void setVec4(const std::string &name, const glm::vec4 &value) const;
		void setMat3(const std::string &name, const glm::mat3 &value) const;
		void setMat4(const std::string &name, const glm::mat4 &value) const;
	};
}