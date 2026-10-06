#pragma once
#include <serialisation/Serialisation.h>
#include <Shader.h>


namespace Tank
{
	class Texture;


	struct CubeMapData
	{
		friend class CubeMapComponent;
	private:
		static ShaderSources s_defaultSources;
	};


	struct CubeMapComponent
	{
		friend class Renderer;
	private:
		unsigned m_vao;
		unsigned m_vbo;
		std::shared_ptr<Texture> m_texture;
		std::array<Resource, 6> m_texturePaths;
	public:
		Shader shader;

		CubeMapComponent(
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
		const std::array<Resource, 6> &getTexPaths() { return m_texturePaths; }

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


	template <>
	json serialise<CubeMapComponent>(CubeMapComponent *);
	template <>
	CubeMapComponent deserialise(const json &);
}
