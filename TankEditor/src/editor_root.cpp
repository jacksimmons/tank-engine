#include <nodes/scene.hpp>
#include "editor_root.hpp"


namespace Tank::Editor
{
	EditorRoot EditorRoot::s_instance;


	void EditorRoot::setScene(std::unique_ptr<Scene> scene)
	{
		std::vector<Scene *> existingScenes = s_instance.m_root->getChildrenOfType<Scene>();
		assert(existingScenes.size() <= 1);
		if (existingScenes.size() > 0) existingScenes[0]->destroy();

		s_instance.m_root->addChild(std::move(scene));
	}
}