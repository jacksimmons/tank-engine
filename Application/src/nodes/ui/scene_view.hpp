#pragma once
#include "nodes/ui/window.hpp"


namespace Tank::Editor
{
	class Tank::Framebuffer;
	class Tank::Camera;
	class Tank::KeyInput;
	/// <summary>
	/// Displays a Scene in a window. Draws a Play button, that when pressed,
	/// calls 'startup' on all Nodes. Is exempt from EditorNode properties.
	/// </summary>
	class _SceneView final : public _Window
	{
		friend class EditorApp;
	private:
		/// <summary>
		/// The viewport size when rendering normally.
		/// </summary>
		int m_sceneW, m_sceneH;
		bool m_isFocussed;
		bool m_isInPlayMode;
		GLenum m_polygonMode;
		GLenum m_cullFaceMode;
		GLenum m_frontFaceMode;
		GLenum m_depthFuncComparisonMode;
		GLenum m_blendFuncSFactor;
		GLenum m_blendFuncDFactor;

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
		void startup() override;
		void update() override;
		void shutdown() override;
		virtual void draw() override;
		virtual void drawPanel() override;
		constexpr int getSceneW() const noexcept { return m_sceneW; }
		constexpr int getSceneH() const noexcept { return m_sceneH; }
		void handleKeyInput();
		void cyclePolygonMode();
		void cycleCullFaceMode();
		void cycleFrontFaceMode();
		void cycleDepthFuncComparisonMode();
		void cycleBlendFuncFactor(GLenum &factor, const std::string &factorName);
	};
}