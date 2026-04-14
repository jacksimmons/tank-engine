#include <nodes/scene.hpp>
#include "editor_root.hpp"


namespace Tank::Editor
{
	EditorRoot EditorRoot::s_instance;


	void EditorRoot::setScene(std::unique_ptr<Scene> scene)
	{
		Node* existingScene = s_instance.m_root->getChild("Scene");
		if (existingScene) existingScene->destroy();

		assert(scene->getName() == "Scene");
		s_instance.m_root->addChild(std::move(scene));
	}
}