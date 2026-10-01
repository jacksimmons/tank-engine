#pragma once


namespace Tank
{
	struct CubeMapComponent;
	struct LightComponent;
	class IMeshContainer;
	class Camera;


	class Renderer
	{
	public:
		static void drawMesh(const Shader &shader, const Mesh &mesh);

		static void beginEditorOutline(const IMeshContainer &outlined);
		static void endEditorOutline(TransformComponent &transform, const IMeshContainer &outlined, const Camera &camera);

		static void drawCubeMap(CubeMapComponent *cubeMap, const Camera &camera);
		static void drawModel(TransformComponent &transform, const ModelComponent &model, const Camera &camera);
	};
}