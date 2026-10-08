#include <format>
#include <glad/glad.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <scene/Scene.h>
#include <Mesh.h>
#include <Log.h>
#include <Texture.h>
#include <Shader.h>
#include "Model.h"
#include "Camera.h"
#include "Light.h"


namespace Tank
{
	ModelComponent::ModelComponent(const Resource &modelPath)
		: IMeshContainer(), shader({}, {})
	{
		setModelPath(modelPath);
		cullFace = GL_BACK;
	}


	ModelComponent::ModelComponent(const ModelComponent &other)
		: IMeshContainer(), shader(other.shader)
	{
		setModelPath(other.m_modelPath);
		cullFace = other.cullFace;
	}


	void ModelComponent::setModelPath(const Resource &res)
	{
		m_meshes.clear();
		m_modelPath = res;
	}


	void ModelComponent::process()
	{
		std::string modelPath = m_modelPath.resolvePathStr();

		// Replace all backslashes with forward slashes (for assimp)
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


	void ModelComponent::processNode(aiNode *node, const aiScene *scene)
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


	std::unique_ptr<Mesh> ModelComponent::processMesh(aiMesh *mesh, const aiScene *scene)
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
			shader.use();
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

			auto diffuse = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
			textures.insert(textures.end(), diffuse.begin(), diffuse.end());

			auto specular = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
			textures.insert(textures.end(), specular.begin(), specular.end());
			shader.unuse();
		}

		return std::make_unique<Mesh>(vertices, indices, textures);
	}


	std::vector<std::shared_ptr<Texture>> ModelComponent::loadMaterialTextures(aiMaterial *mat, int assimpTexType, std::string typeName)
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
				fs::path texturePath = m_modelPath.resolvePath().parent_path() / str.C_Str();
				auto tex = Texture::fromFile(texturePath, typeName);

				if (tex.has_value())
				{
					std::shared_ptr<Texture> val = tex.value();
					textures.push_back(val);
					Texture::addLoadedTexture(val);
				}
				else
				{
					TE_CORE_ERROR(std::format("Unable to load texture {}", texturePath.string()));
				}
			}
		}

		return textures;
	}
}
