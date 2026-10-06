#include <glad/glad.h>
#include "Shader.h"


namespace Tank
{
	json Shader::serialise(const Shader &shader)
	{
		json serialised = ShaderSources::serialise(shader.m_sources);
		serialised["id"] = shader.m_id;
		return serialised;
	}


	Shader *Shader::deserialise(const json &serialised)
	{
		return new Shader(serialised["id"], ShaderSources::deserialise(serialised));
	}


	Shader::Shader(std::optional<unsigned> id, const ShaderSources &sources) : m_sources(sources)
	{
		if (id.has_value())
			m_id = id.value();
		else
			m_id = glCreateProgram();

		if (!attachShader(m_id, m_sources.vertex))
			TE_CORE_ERROR(std::format("Failed to load vertex source: {}", sources.vertex.location.encode()));
		if (!attachShader(m_id, m_sources.fragment))
			TE_CORE_ERROR(std::format("Failed to load fragment source: {}", sources.fragment.location.encode()));
		if (sources.geometry.enabled && !attachShader(m_id, m_sources.geometry))
			TE_CORE_ERROR(std::format("Failed to load geometry source: {}", sources.geometry.location.encode()));

		glLinkProgram(m_id);
	}


	Shader::~Shader()
	{
		glDeleteProgram(m_id);
	}


	bool Shader::attachShader(unsigned programID, ShaderSource &source)
	{
		std::string shaderContents;

		// Exit with error if any shader file cannot be read.
		if (!readShaderFile(source.location.resolvePath(), shaderContents, "Unspecified shader")) return false;

		std::optional<GLuint> shader = compileShader(shaderContents, source.glType, "Unspecified shader");

		// Exit with error if any shader file fails to compile.
		if (!shader.has_value()) return false;
		source.glID = shader.value();

		glAttachShader(programID, shader.value());
		return true;
	}


	bool Shader::readShaderFile(const fs::path &shaderPath, std::string &shaderContents, const std::string &shaderType)
	{
		if (File::readLines(shaderPath, shaderContents) != File::ReadResult::Success)
		{
			std::string errMsg = "Failed to read " + shaderType + " shader: " + shaderPath.string();
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