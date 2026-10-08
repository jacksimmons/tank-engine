#pragma once
#include <nodes/interfaces/MeshContainer.h>
#include <serialisation/Serialisation.h>
#include <Shader.h>


struct aiNode; struct aiScene; struct aiMesh;
struct aiMaterial;
namespace Tank
{
	class Mesh;
	class Texture;


	/// <summary>
	/// A class which can load 3D models using Assimp.
	/// </summary>
	struct ModelComponent : public IMeshContainer
	{
	private:
		Resource m_modelPath;
		unsigned m_cullFace;
	public:
		Shader shader;
		unsigned cullFace;

		ModelComponent(const Resource &modelPath = Res("models/backpack/backpack.obj", true));
		ModelComponent(const ModelComponent &model);
		~ModelComponent() = default;

		void setModelPath(const Resource &resource);
		const Resource &getModelPath() const { return m_modelPath; }

		void process();
		void processNode(aiNode *node, const aiScene *scene);
		std::unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
		
		std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, int assimpTextureType, std::string typeName);
	};
}
