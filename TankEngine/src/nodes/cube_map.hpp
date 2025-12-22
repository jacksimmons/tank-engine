#pragma once
#include <nodes/node.hpp>
#include <nodes/interfaces/shader_container.hpp>


namespace Tank
{
	class Node;
	class Texture;
	class Shader;
	struct ShaderSource;
	struct ShaderSources;
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
		std::array<Resource, 6> m_texturePaths;
	public:
		CubeMap(
			const std::string &name = "CubeMap",
			const std::array<Resource, 6> &textureNames =
			{
				Res("textures/skybox/right.jpg", true),
				Res("textures/skybox/left.jpg", true),
				Res("textures/skybox/bottom.jpg", true),
				Res("textures/skybox/top.jpg", true),
				Res("textures/skybox/front.jpg", true),
				Res("textures/skybox/back.jpg", true)
			}
		);

		void setTexPaths(const std::array<Resource, 6> &texPaths);
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