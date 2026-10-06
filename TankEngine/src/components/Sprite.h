#pragma once
#include <nodes/interfaces/MeshContainer.h>
#include <serialisation/Serialisation.h>
#include <Shader.h>


namespace Tank
{
	struct SpriteComponent : public IMeshContainer
	{
	private:
		Resource m_texPath;
	public:
		Shader shader;

		SpriteComponent(const Resource &texPath = Resource("textures/awesomeface.png", true));
		SpriteComponent(const SpriteComponent &sc) = default;
		~SpriteComponent() = default;

		bool setTexPath(const Resource &texPath);
		const Resource& getTexPath() { return m_texPath; }
	};


	template <>
	json serialise<SpriteComponent>(SpriteComponent *);
	template <>
	SpriteComponent deserialise(const json &);
}
