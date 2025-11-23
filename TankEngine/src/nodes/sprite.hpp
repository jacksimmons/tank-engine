#pragma once
#include "interfaces/mesh_container.hpp"
namespace fs = std::filesystem;


namespace Tank
{
	namespace Reflect { class NodeFactory; }

	class TANK_API Sprite : public Node, public IMeshContainer
	{
	public:
		virtual json serialise() override;
		static void deserialise(const json &serialised, Sprite **targetPtr);
	private:
		fs::path m_texPath;
	public:
		Sprite(const std::string &name,
			ShaderSources &sources,
			const fs::path &texPath
		);
		virtual ~Sprite() = default;

		void setTexPath(const fs::path &texPath);
		const fs::path& getTexPath() { return m_texPath; }
		virtual void draw() override;
	};
}