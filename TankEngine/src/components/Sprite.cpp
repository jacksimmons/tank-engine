#include <glm/gtc/matrix_inverse.hpp>
#include <scene/Scene.h>
#include <Shader.h>
#include <Texture.h>
#include <QuadMesh.h>
#include "Sprite.h"
#include "Light.h"
#include "Camera.h"


namespace Tank
{
	SpriteComponent::SpriteComponent(const Resource &texPath)
		: IMeshContainer(), shader({}, {})
	{
		setTexPath(texPath);
	}


	bool SpriteComponent::setTexPath(const Resource &texPath)
	{
		const auto &tex = Texture::fromFile(texPath.resolvePath(), "diffuse");
		m_texPath = texPath;

		m_meshes.clear();
		if (tex.has_value())
		{
			m_meshes.push_back(std::unique_ptr<QuadMesh>(new QuadMesh({ tex.value() })));
			return true;
		}
		else
		{
			TE_CORE_ERROR(std::format("Couldn't decode texture at {}", texPath.resolvePathStr()));
			return false;
		}
	}


	// =======================
	//		Serialisation
	// =======================
	template <>
	json serialise<SpriteComponent>(SpriteComponent *in)
	{
		json serialised;
		serialised["texPath"] = Res::encode(in->getTexPath());
		serialised["shader"] = Shader::serialise(in->shader);
		return serialised;
	}

	template <>
	SpriteComponent deserialise(const json &serialised)
	{
		SpriteComponent sc {};
		sc.shader = Shader { {}, ShaderSources::deserialise(serialised["shader"]) };
		sc.setTexPath(Res::decode(serialised["texPath"]));
		return sc;
	}
}
