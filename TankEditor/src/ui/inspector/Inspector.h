#pragma once
#include "ui/Window.h"
#include "NodeInspectors/NodeInspector.h"


namespace Tank
{
	class Scene; class Model; class SceneCamera; class Light; class DirLight; class PointLight;
	class IShaderContainer; class ShaderSource; class IMeshContainer; class Sprite; class Model;
}
namespace Tank::Editor
{
	class Inspector_ final : public _Window
	{
		friend class EditorApp;
		friend class ProjectMenuBar_;
	private:
		GameEntity *m_selectedEntity;
		std::vector<std::unique_ptr<NodeInspector_Base>> m_nodeInspectors;

		Inspector_(const std::string &name = "Inspector");

		template <class T>
		void tryAddSection();
	protected:
		virtual void drawPanel() override;
	public:
		GameEntity *selected() const noexcept { return m_selectedEntity; }
	};
}