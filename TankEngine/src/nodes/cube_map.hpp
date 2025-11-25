#pragma once
#include "tepch.hpp"
#include "shader_source.hpp"
#include "nodes/node.hpp"
#include <nodes/interfaces/shader_container.hpp>


namespace Tank
{
	class Node;
	class Texture;
	class Shader;
	namespace Reflect { class NodeFactory; }

	class TANK_API CubeMap : public Node, public IShaderContainer
	{
		// Serialisation
	public:
		virtual json serialise() override;
		virtual void deserialise(const json &serialised) override;

	protected:
		unsigned m_vao;
		unsigned m_vbo;
		std::shared_ptr<Texture> m_texture;
		std::array<std::string, 6> m_texturePaths;
	public:
		CubeMap(
			const std::string &name = "CubeMap",
			ShaderSources *sources = nullptr,
			const std::array<std::string, 6> &textureNames = { "skybox/right.jpg", "skybox/left.jpg", "skybox/bottom.jpg", "skybox/top.jpg", "skybox/front.jpg", "skybox/back.jpg" }
		);

		void setTexPaths(const std::array<std::string, 6> &texPaths);
		virtual void draw() override;

	private:
		constexpr static float s_vertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
	};
}