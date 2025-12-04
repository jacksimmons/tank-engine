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
	class _Inspector final : public _Window
	{
		friend class EditorApp;
	private:
		Node *m_inspectedNode;
		std::unique_ptr<_NodeInspectorBase> m_nodeInspector;

		_Inspector(const std::string &name = "Inspector");
		template <class T>
		void tryDrawSection();
	protected:
		virtual void drawPanel() override;
	public:
		virtual void onAdopted() override;
		Node *getInspectedNode() const noexcept { return m_inspectedNode; }
	};
}