#include <glm/gtc/matrix_inverse.hpp>
#include <shader_source.hpp>
#include <shader.hpp>
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
		serialised["texPath"] = Res::encode(m_texPath);
		serialised["shader"] = Shader::serialise(getShader());
		return serialised;
	}


	void Sprite::deserialise(const json &serialised)
	{
		initShaderContainer(ShaderSources::deserialise(serialised["shader"]));
		setTexPath(Res::decode(serialised["texPath"]));
		
		Node::deserialise(serialised);
	}


	Sprite::Sprite(
		const std::string &name,
		const Resource &texPath
	): Node(name), IMeshContainer()
	{
		m_type = "Sprite";
		setTexPath(texPath);
	}


	bool Sprite::setTexPath(const Resource &texPath)
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


	void Sprite::draw()
	{
		if (!Visible()) return;

		IOutlined::predraw();

		const Shader &shader = getShader();
		shader.use();
		
		shader.setVec3("tex_scale", glm::vec3{ 1, 1, 1 });
		shader.setFloat("material.Ns", 32.0f);

		auto cam = Scene::getActiveScene()->getActiveCamera();
		auto P = cam->getProj();
		auto V = cam->getView();
		auto M = getTransform()->getWorldModelMatrix();
		auto VM = V * M;
		
		shader.setMat4("PVM", P * VM);
		shader.setMat4("VM", VM);
		shader.setMat4("V", V);
		shader.setMat4("VM_it", glm::inverseTranspose(VM));

		auto scene = Scene::getActiveScene();
		auto activeLights = scene->getLights();
		for (Light *light : activeLights)
		{
			light->updateShader(shader);
		}
		
		shader.setInt("num_dir_lights", scene->getNumLights(LightType::Directional));
		shader.setInt("num_point_lights", scene->getNumLights(LightType::Point));

		for (unsigned i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i]->draw(shader);
		}

		shader.unuse();
		
		IOutlined::postdraw(m_transform.get());
		Node::draw();
	}
}