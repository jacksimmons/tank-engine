#pragma once
#include "ui/window.hpp"


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
		friend class _Hierarchy;
	private:
		Node *m_inspectedNode;

		_Inspector(const std::string &name = "Inspector");
		template <class T>
		void tryDrawSection();
		void onNodeDeleted(Node *node);
		void setInspectedNode(Node *node);
	protected:
		virtual void drawPanel() override;
	public:
		Node *getInspectedNode() const noexcept { return m_inspectedNode; }
	};
}