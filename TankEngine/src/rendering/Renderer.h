#pragma once


namespace Tank
{
	struct CubeMapComponent;
	struct LightComponent;
	class IMeshContainer;
	class SceneCamera;


	class Renderer
	{
	public:
		static void drawMesh(const Shader &shader, const Mesh &mesh);

		static void beginEditorOutline(const IMeshContainer &outlined);
		static void endEditorOutline(TransformComponent &transform, const IMeshContainer &outlined, const SceneCamera &camera);

		static void drawCubeMap(CubeMapComponent *cubeMap, const SceneCamera &camera);
		static void drawModel(TransformComponent &transform, const ModelComponent &model, const SceneCamera &camera);
	};
}