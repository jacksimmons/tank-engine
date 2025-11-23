#include <glm/gtc/matrix_inverse.hpp>
#include <glad/glad.h>
#include "sprite.hpp"
#include "quad_mesh.hpp"
#include "texture.hpp"
#include "light.hpp"
#include "nodes/scene.hpp"
#include "nodes/camera.hpp"
#include "../reflection/node_factory.hpp"
namespace fs = std::filesystem;


namespace Tank
{
	json Sprite::serialise()
	{
		json serialised = Node::serialise();
		serialised["texPath"] = m_texPath.string();
		serialised["shader"] = Shader::serialise(*(m_shader));
		return serialised;
	}


	void Sprite::deserialise(const json &serialised, Sprite **targetPtr)
	{
		ShaderSources sources;
		sources.vertex.location = std::string{ serialised["shader"]["vert"] };
		sources.fragment.location = std::string{ serialised["shader"]["frag"] };
		sources.geometry.location = std::string{ serialised["shader"]["geom"] };
		if (!(*targetPtr)) *targetPtr = new Sprite(serialised["name"], sources, serialised["texPath"]);

		Node *target = *targetPtr;
		target->deserialise(serialised);
	}


	Sprite::Sprite(const std::string &name,
		ShaderSources &sources,
		const fs::path &texPath)
		: Node(name), IMeshContainer(sources)
	{
		m_type = "Sprite";

		setTexPath(texPath);
	}


	void Sprite::setTexPath(const fs::path &texPath)
	{
		const auto &tex = Texture::fromFile(texPath, "diffuse");
		m_texPath = texPath;

		m_meshes.clear();
		if (tex.has_value())
		{
			m_meshes.push_back(std::unique_ptr<QuadMesh>(new QuadMesh({ tex.value() })));
		}
	}


	void Sprite::draw()
	{
		if (!getVisibility()) return;

		IOutlined::predraw();
		m_shader->use();

		m_shader->setVec3("tex_scale", glm::vec3{ 1, 1, 1 });
		m_shader->setFloat("material.Ns", 32.0f);

		auto cam = Scene::getActiveScene()->getActiveCamera();
		auto P = cam->getProj();
		auto V = cam->getView();
		auto M = Node::getTransform()->getWorldModelMatrix();
		auto VM = V * M;
		m_shader->setMat4("PVM", P * VM);
		m_shader->setMat4("VM", VM);
		m_shader->setMat4("V", V);
		m_shader->setMat4("VM_it", glm::inverseTranspose(VM));

		auto scene = Scene::getActiveScene();
		auto activeLights = scene->getLights();
		for (Light *light : activeLights)
		{
			light->updateShader(m_shader.get());
		}
		m_shader->setInt("num_dir_lights", scene->getNumLights(LightType::Directional));
		m_shader->setInt("num_point_lights", scene->getNumLights(LightType::Point));

		for (unsigned i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i]->draw(m_shader.get());
		}
		m_shader->unuse();
		IOutlined::postdraw(m_transform.get());

		Node::draw();
	}
}