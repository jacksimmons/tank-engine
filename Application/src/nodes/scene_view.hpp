#pragma once
#include <memory>
#include <glad/glad.h>
#include "nodes/node.hpp"


namespace Tank
{
	class Framebuffer;
	class SceneView : public Node
	{
	private:
		/// <summary>
		/// The viewport size when rendering normally.
		/// </summary>
		int m_sceneW, m_sceneH;

		std::unique_ptr<Framebuffer> m_fb;
	public:
		SceneView(std::string name, glm::ivec2 stdViewportSize, glm::ivec2 fbViewportSize);
		void rescale(int w, int h) const;
		void draw() const override;
		constexpr int getSceneW() const noexcept { return m_sceneW; }
		constexpr int getSceneH() const noexcept { return m_sceneH; }
	};
}