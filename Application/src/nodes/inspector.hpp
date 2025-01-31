#pragma once
#include <memory>
#include "nodes/ui/panel.hpp"


namespace Tank
{
	class Model; class Camera; class Light; class DirLight;
	class Inspector : public Panel
	{
		friend class Hierarchy;
	private:
		Node *m_inspectedNode;

	private:
		void drawNodeSection() const;
		void drawModelSection(Model *model) const;
		void drawCameraSection(Camera *camera) const;
		void drawLightSection(Light *light) const;
		void drawDirLightSection(DirLight *dir) const;
		void onNodeDeleted(Node *node);
	protected:
		virtual void drawPanel() override;
	public:
		Inspector(const std::string &name);

		void setInspectedNode(Node *node) noexcept { m_inspectedNode = node; }
		Node *getInspectedNode() const noexcept { return m_inspectedNode; }
	};
}