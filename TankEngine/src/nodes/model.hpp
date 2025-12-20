#pragma once
#include <nodes/node.hpp>
#include <nodes/interfaces/mesh_container.hpp>


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
	class TANK_API Model : public Node, public IMeshContainer
	{
	public:
		virtual json serialise() override;
		void deserialise(const json &serialised);

	private:
		fs::path m_modelDirectory;
		fs::path m_modelFile;
		unsigned m_cullFace;
	public:
		Model(
			const std::string &name = "Model",
			const fs::path &modelPath = "models/backpack/backpack.obj",
			ShaderSources *sources = nullptr
		);
		virtual ~Model() = default;

		void setModelPath(const fs::path &path);
		fs::path getModelPath() const { return m_modelDirectory / m_modelFile; }

		unsigned getCullFace() const { return m_cullFace; }
		void setCullFace(unsigned face) { m_cullFace = face; }

		virtual void draw() override;
		virtual void update() override;
		void processNode(aiNode *node, const aiScene *scene);
		std::unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
		void processLights();
		
		std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, int assimpTextureType, std::string typeName);
	};
}
