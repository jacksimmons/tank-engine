#pragma once

#include "interfaces/mesh_container.hpp"
namespace fs = std::filesystem;


namespace Tank
{
	class Sprite : public IMeshContainer
	{
	public:
		virtual json serialise() override;
		static void deserialise(const json &serialised, Sprite **targetPtr);
	private:
		fs::path m_spritePath;
	public:
		Sprite(const std::string &name,
			ShaderSources &sources,
			const fs::path &spritePath
		);
		virtual ~Sprite() = default;

		virtual void draw() override;
	};
}