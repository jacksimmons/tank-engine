#pragma once
#include <nodes/node.hpp>
#include "interfaces/mesh_container.hpp"
namespace fs = std::filesystem;


namespace Tank
{
	namespace Reflect { class NodeFactory; }


	class TANK_API Sprite : public Node, public IMeshContainer
	{
	public:
		virtual json serialise() override;
		void deserialise(const json &serialised);
	private:
		fs::path m_texPath;
	public:
		Sprite(
			const std::string &name = "Sprite",
			const fs::path &texPath = "textures/awesomeface.png",
			ShaderSources *sources = nullptr
		);
		virtual ~Sprite() = default;

		void setTexPath(const fs::path &texPath);
		const fs::path& getTexPath() { return m_texPath; }
		virtual void draw() override;
	};
}