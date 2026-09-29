#pragma once
#include <nodes/interfaces/MeshContainer.h>
#include <serialisation/Serialisation.h>


namespace Tank
{
	namespace Reflect { class NodeFactory; }


	struct SpriteComponent : public IMeshContainer
	{
	private:
		Resource m_texPath;
	public:
		SpriteComponent(const Resource &texPath = Resource("textures/awesomeface.png", true));
		virtual ~SpriteComponent() = default;

		bool setTexPath(const Resource &texPath);
		const Resource& getTexPath() { return m_texPath; }
	};


	template <>
	json serialise<SpriteComponent>(SpriteComponent *);
	template <>
	void deserialise<SpriteComponent>(const json &, SpriteComponent *);
}
