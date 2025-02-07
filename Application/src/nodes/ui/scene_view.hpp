#pragma once
#include <memory>
#include <glad/glad.h>
#include "nodes/ui/panel.hpp"


namespace Tank::Editor
{
	class Tank::Framebuffer;
	class Tank::Camera;
	class Tank::KeyInput;
	class _SceneView : public _Panel
	{
		friend class EditorApp;
	private:
		/// <summary>
		/// The viewport size when rendering normally.
		/// </summary>
		int m_sceneW, m_sceneH;
		bool m_isFocussed;
		GLenum m_polygonMode;

		std::unique_ptr<Framebuffer> m_fb;
		KeyInput *m_keyInput;
		
		// Properties for updating FPS counter at a constant frequency,
		// rather than every frame.
		float m_fpsDisplayUpdateTimer = 0;
		std::string m_fpsDisplayLastText = "";
		const float FPS_DISPLAY_UPDATE_FREQUENCY = 0.5f;

		_SceneView(const std::string &name,
			glm::ivec2 stdViewportSize,
			glm::ivec2 fbViewportSize,
			KeyInput *keyInput
		);
	public:
		void rescale(int w, int h) const;
		void update() override;
		virtual void drawUI() override;
		virtual void drawPanel() override;
		constexpr int getSceneW() const noexcept { return m_sceneW; }
		constexpr int getSceneH() const noexcept { return m_sceneH; }
		void handleKeyInput();
		void cyclePolygonMode();
	};
}