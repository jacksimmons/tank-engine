#pragma once
#include "ui/window.hpp"
#include "node_inspectors/node_inspector.hpp"


namespace Tank
{
	class Scene; class Model; class Camera; class Light; class DirLight; class PointLight;
	class IShaderContainer; class ShaderSource; class IMeshContainer; class Sprite; class Model;
}
namespace Tank::Editor
{
	class Inspector_ final : public _Window
	{
		friend class EditorApp;
		friend class ProjectMenuBar_;
	private:
		Node *m_inspectedNode;
		std::vector<std::unique_ptr<NodeInspector_Base>> m_nodeInspectors;

		Inspector_(const std::string &name = "Inspector");

		template <class T>
		void tryAddSection();
	protected:
		virtual void drawPanel() override;
	public:
		Node *getInspectedNode() const noexcept { return m_inspectedNode; }
	};
}