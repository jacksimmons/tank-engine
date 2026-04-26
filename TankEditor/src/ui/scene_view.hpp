#pragma once
#include "ui/window.hpp"
#include "framebuffer.hpp"


namespace Tank
{
	class Camera;
	class KeyInput;
}
namespace Tank::Editor
{
	/// <summary>
	/// Displays a Scene in a window. Draws a Play button, that when pressed,
	/// calls 'startup' on all Nodes. Is exempt from EditorNode properties.
	/// </summary>
	class SceneView_ final : public _Window
	{
		friend class ProjectMenuBar_;
		friend class EditorApp;

	private:
		/// <summary>
		/// The viewport size when rendering normally.
		/// </summary>
		int m_sceneW, m_sceneH;
		bool m_isFocussed;
		bool m_isInPlayMode;
		bool m_isPlayer;
		unsigned m_polygonMode;
		unsigned m_cullFaceMode;
		unsigned m_frontFaceMode;
		unsigned m_depthFuncComparisonMode;
		unsigned m_blendFuncSFactor;
		unsigned m_blendFuncDFactor;

		std::unique_ptr<Framebuffer> m_fb;
		Tank::KeyInput *m_keyInput;

		// Properties for updating FPS counter at a constant frequency,
		// rather than every frame.
		float m_fpsDisplayUpdateTimer = 0;
		std::string m_fpsDisplayLastText = "";
		const float FPS_DISPLAY_UPDATE_FREQUENCY = 0.5f;
	public:
		SceneView_(const std::string &name,
			glm::ivec2 stdViewportSize,
			glm::ivec2 fbViewportSize,
			Tank::KeyInput *keyInput,
			bool alwaysPlayMode = false
		);

		void rescale(int w, int h) const;
		void update() override;
		virtual void draw() override;
		virtual void drawPanel() override;
		constexpr int getSceneW() const noexcept { return m_sceneW; }
		constexpr int getSceneH() const noexcept { return m_sceneH; }
		void handleKeyInput();
		void cyclePolygonMode();
		void cycleCullFaceMode();
		void cycleFrontFaceMode();
		void cycleDepthFuncComparisonMode();
		void cycleBlendFuncFactor(unsigned &factor, const std::string &factorName);
	};
}
