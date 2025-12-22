#include <glad/glad.h>
#include "shader_source.hpp"


namespace Tank
{
	ShaderSource::ShaderSource()
	{
		glType = GL_VERTEX_SHADER;
		glID = 0;
		location = Resource("", false);
		enabled = false;
	}


	ShaderSource::ShaderSource(const ShaderSource &original)
	{
		glType = original.glType;
		glID = original.glID;
		location = original.location; // fs::path &operator=(const fs::path &) is a copy constructor
		enabled = original.enabled;
	}


	bool ShaderSource::operator==(const ShaderSource &other)
	{
		return glType == other.glType
			&& glID == other.glID
			&& location == other.location
			&& enabled == other.enabled;
	}


	ShaderSources::ShaderSources()
	{
		vertex.glType = GL_VERTEX_SHADER;
		vertex.enabled = true;
		vertex.location = Res("shaders/shader.vert", true);

		fragment.glType = GL_FRAGMENT_SHADER;
		fragment.enabled = true;
		fragment.location = Res("shaders/shader.frag", true);

		geometry.glType = GL_GEOMETRY_SHADER;
	}


	ShaderSources::ShaderSources(const ShaderSources &original)
	{
		vertex = original.vertex;
		fragment = original.fragment;
		geometry = original.geometry;
	}


	bool ShaderSources::operator==(const ShaderSources &other)
	{
		return vertex == other.vertex && fragment == other.fragment && geometry == other.geometry;
	}
}