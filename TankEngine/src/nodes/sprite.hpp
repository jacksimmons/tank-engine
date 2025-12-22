#pragma once
#include <nodes/node.hpp>
#include "interfaces/mesh_container.hpp"


namespace Tank
{
	namespace Reflect { class NodeFactory; }


	class TANK_API Sprite : public Node, public IMeshContainer
	{
	public:
		virtual json serialise() override;
		void deserialise(const json &serialised);
	private:
		Resource m_texPath;
	public:
		Sprite(
			const std::string &name = "Sprite",
			const Resource &texPath = Resource("textures/awesomeface.png", true)
		);
		virtual ~Sprite() = default;

		bool setTexPath(const Resource &texPath);
		const Resource& getTexPath() { return m_texPath; }
		virtual void draw() override;
	};
}