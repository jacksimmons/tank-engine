#include <glad/glad.h>
#include "shader.hpp"


namespace Tank
{
	json Shader::serialise(const Shader &shader)
	{
		json serialised;
		serialised["id"] = shader.m_id;
		serialised["vert"] = shader.m_sources.vertex.location;
		serialised["frag"] = shader.m_sources.fragment.location;
		serialised["geom"] = shader.m_sources.geometry.location;
		return serialised;
	}


	Shader *Shader::deserialise(const json &serialised)
	{
		ShaderSources sources;
		sources.vertex.location = std::string{ serialised["vert"] };
		sources.fragment.location = std::string { serialised["frag"] };
		sources.geometry.location = std::string{ serialised["geom"] };

		return new Shader(serialised["id"], sources);
	}


	Shader::Shader(unsigned id, const ShaderSources &sources) : m_id(id), m_sources(sources)
	{

	}


	Shader::~Shader()
	{
		glDeleteProgram(m_id);
	}


	std::optional<std::unique_ptr<Shader>> Shader::createShader(ShaderSources &sources)
	{
		std::string vsString, fsString, gsString;
		unsigned progId = glCreateProgram();
		std::vector<GLuint> shadersToDelete;

		if (!attachShader(progId, sources.vertex)) return {};
		if (!attachShader(progId, sources.fragment)) return {};
		if (sources.geometry.enabled && !attachShader(progId, sources.geometry)) return {};

		glLinkProgram(progId);

		// Cleanup shaders
		for (GLuint shader : shadersToDelete)
		{
			glDeleteProgram(shader);
		}

		return std::unique_ptr<Shader>(new Shader(progId, sources));
	}


	bool Shader::attachShader(unsigned programID, ShaderSource &source)
	{
		std::string shaderContents;

		// Exit with error if any shader file cannot be read.
		if (!readShaderFile(source.location, shaderContents, "Unspecified shader")) return false;

		std::optional<GLuint> shader = compileShader(shaderContents, source.glType, "Unspecified shader");

		// Exit with error if any shader file fails to compile.
		if (!shader.has_value()) return false;
		source.glID = shader.value();

		glAttachShader(programID, shader.value());
		return true;
	}


	bool Shader::readShaderFile(const fs::path &shaderPath, std::string &shaderContents, const std::string &shaderType)
	{
		if (File::readLines(std::filesystem::path(ROOT_DIRECTORY) / "shaders" / shaderPath, shaderContents) != File::ReadResult::Success)
		{
			std::string errMsg = "Failed to read " + shaderType + " shader: " + std::string(ROOT_DIRECTORY) +
				std::string("/shaders/") + shaderPath.string();
			TE_CORE_ERROR(errMsg);
			return false;
		}

		return true;
	}


	std::optional<unsigned> Shader::compileShader(const std::string &shaderContents, unsigned shaderType, const std::string &shaderTypeStr)
	{
		unsigned int shader;
		shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, StringHelper(shaderContents), nullptr);
		glCompileShader(shader);

		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			TE_CORE_ERROR("Failed to compile " + shaderTypeStr + " shader.");
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			TE_CORE_ERROR(std::string("Shader info log: ") + infoLog);

			glDeleteShader(shader);
			return {};
		}
		return shader;
	}


	int Shader::getLoc(const std::string &name) const
	{
		return glGetUniformLocation(m_id, name.c_str());
	}


	void Shader::use() const
	{
		glUseProgram(m_id);
	}


	void Shader::unuse() const
	{
		glUseProgram(0);
	}


	void Shader::setInt(const std::string &name, int value) const
	{
		glUniform1i(getLoc(name), value);
	}


	void Shader::setFloat(const std::string &name, float value) const
	{
		glUniform1f(getLoc(name), value);
	}


	void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(getLoc(name), 1, &value[0]);
	}


	void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(getLoc(name), 1, &value[0]);
	}


	void Shader::setMat3(const std::string &name, const glm::mat3 &value) const
	{
		glUniformMatrix3fv(getLoc(name), 1, GL_FALSE, glm::value_ptr(value));
	}


	void Shader::setMat4(const std::string &name, const glm::mat4 &value) const
	{
		glUniformMatrix4fv(getLoc(name), 1, GL_FALSE, glm::value_ptr(value));
	}
}