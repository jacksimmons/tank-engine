#pragma once

#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "interfaces/mesh_container.hpp"


struct aiNode; struct aiScene; struct aiMesh;
struct aiMaterial; enum aiTextureType;
namespace Tank
{
	class Texture;

	/// <summary>
	/// A class which can load 3D models using Assimp.
	/// </summary>
	class Model : public IMeshContainer
	{
	public:
		static json serialise(Model *model);
		static void deserialise(const json &serialised, Model **targetPtr);

	private:
		std::string m_modelDirectory;
		std::string m_modelFile;
	public:
		Model(const std::string &name,
			ShaderSources &sources,
			const std::string &modelPath
		);
		virtual ~Model() = default;

		virtual void draw() override;
		virtual void update() override;
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		void processLights();

		std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	};
}