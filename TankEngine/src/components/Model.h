#pragma once
#include <nodes/interfaces/MeshContainer.h>
#include <serialisation/Serialisation.h>


struct aiNode; struct aiScene; struct aiMesh;
struct aiMaterial;
namespace Tank
{
	class Mesh;
	class Texture;
	struct ShaderSources;
	namespace Reflect { class NodeFactory; }


	/// <summary>
	/// A class which can load 3D models using Assimp.
	/// </summary>
	struct ModelComponent : public IMeshContainer
	{
	private:
		Resource m_modelPath;
		unsigned m_cullFace;
	public:
		ModelComponent() = default;
		ModelComponent(const Resource &modelPath = Res("models/backpack/backpack.obj", true));
		virtual ~ModelComponent() = default;

		void setModelPath(const Resource &resource);
		const Resource &getModelPath() const { return m_modelPath; }

		unsigned getCullFace() const { return m_cullFace; }
		void setCullFace(unsigned face) { m_cullFace = face; }

		void draw();
		void update();

		void process();
		void processNode(aiNode *node, const aiScene *scene);
		std::unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
		void processLights();
		
		std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, int assimpTextureType, std::string typeName);
	};

	template <>
	json serialise<ModelComponent>(ModelComponent *);
	template <>
	void deserialise<ModelComponent>(const json &, ModelComponent *);
	using Model = ModelComponent;
}
