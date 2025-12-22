#include <format>
#include <glad/glad.h>
#include <glm/gtc/matrix_inverse.hpp>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/material.h"
#include <mesh.hpp>
#include "log.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "nodes/scene.hpp"
#include "nodes/model.hpp"
#include "nodes/camera.hpp"
#include "nodes/light.hpp"


namespace Tank
{
	json Model::serialise()
	{
		json serialised = Node::serialise();
		serialised["modelPath"] = Resource::encode(getModelPath());
		serialised["shader"] = Shader::serialise(getShader());
		serialised["cullFace"] = getCullFace();
		return serialised;
	}


	void Model::deserialise(const json &serialised)
	{
		initShaderContainer(ShaderSources::deserialise(serialised["shader"]));
		setModelPath(Resource::decode(serialised["modelPath"]));
		setCullFace(serialised["cullFace"]);
		Node::deserialise(serialised);

		process();
	}


	Model::Model(const std::string &name, const Resource &modelPath)
		: Node(name), IMeshContainer()
	{
		m_type = "Model";
		setModelPath(modelPath);
		m_cullFace = GL_BACK;
	}


	void Model::setModelPath(const Resource &res)
	{
		m_meshes.clear();
		m_modelPath = res;
	}


	void Model::process()
	{
		std::string modelPath = m_modelPath.resolvePathStr();

		// Replace all backslashes with forward slashes
		std::replace(modelPath.begin(), modelPath.end(), '\\', '/');

		// Find the last forward slash, to distinguish between directory and filename
		size_t indexOfLastSlash = modelPath.find_last_of("/");

		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			TE_CORE_ERROR(std::format("ASSIMP: {}", importer.GetErrorString()));
			return;
		}

		processNode(scene->mRootNode, scene);
	}


	void Model::processNode(aiNode *node, const aiScene *scene)
	{
		// Process all of node's meshes
		for (unsigned i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(std::move(processMesh(mesh, scene)));
		}

		// Recurse on child meshes
		for (unsigned i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}


	std::unique_ptr<Mesh> Model::processMesh(aiMesh *mesh, const aiScene *scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned> indices;
		std::vector<std::shared_ptr<Texture>> textures;

		// Vertices
		for (unsigned i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vert;
			vert.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vert.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

			// If mesh has tex coords
			if (mesh->mTextureCoords[0])
			{
				vert.texCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			}
			else
			{
				vert.texCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vert);
		}

		// Indices
		for (unsigned i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Materials
		if (mesh->mMaterialIndex >= 0)
		{
			getShader().use();
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

			auto diffuse = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
			textures.insert(textures.end(), diffuse.begin(), diffuse.end());

			auto specular = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
			textures.insert(textures.end(), specular.begin(), specular.end());
			getShader().unuse();
		}

		return std::make_unique<Mesh>(vertices, indices, textures);
	}


	std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial *mat, int assimpTexType, std::string typeName)
	{
		aiTextureType type = (aiTextureType)assimpTexType;

		std::vector<std::shared_ptr<Texture>> textures;

		for (unsigned i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skipLoading = false;

			// See if texture with same path has already been loaded. If it has, copy existing version.
			std::vector<std::shared_ptr<Texture>> loadedTextures = Texture::getLoadedTextures();
			for (unsigned j = 0; j < loadedTextures.size(); j++)
			{
				std::shared_ptr<Texture> loadedTex = loadedTextures[j];
				if (loadedTex->getPath().filename() == str.C_Str())
				{
					textures.push_back(loadedTex);
					skipLoading = true;
					break;
				}
			}

			if (!skipLoading)
			{
				TE_CORE_INFO(m_modelPath.resolvePathStr());
				auto tex = Texture::fromFile(m_modelPath.resolvePath().parent_path() / str.C_Str(), typeName);

				if (tex.has_value())
				{
					std::shared_ptr<Texture> val = tex.value();
					textures.push_back(val);
					Texture::addLoadedTexture(val);
				}
				else
				{
					TE_CORE_ERROR(std::format("Unable to load texture {}", (m_modelPath.resolvePath() / str.C_Str()).string()));
				}
			}
		}

		return textures;
	}


	void Model::draw()
	{
		if (!Visible()) return;

		glCullFace(m_cullFace);

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
		
		processLights();

		for (unsigned i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i]->draw(shader);
		}
		shader.unuse();

		IOutlined::postdraw(m_transform.get());

		glCullFace(GL_BACK);
	}


	void Model::processLights()
	{
		auto scene = Scene::getActiveScene();
		auto activeLights = scene->getLights();

		const Shader &shader = getShader();
		for (Light *light : activeLights)
		{
			light->updateShader(shader);
		}

		shader.setInt("num_dir_lights", scene->getNumLights(LightType::Directional));
		shader.setInt("num_point_lights", scene->getNumLights(LightType::Point));
	}


	void Model::update()
	{
		Node::update();
	}
}
