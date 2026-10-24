#include "EditorEntity.h"


namespace Tank
{
	void EditorEntity::startup()
	{
		m_visible = false;
		for (auto *entity : children())
		{
			entity->setVisible(false);
		}
		GameEntity::startup();
	}


	void EditorEntity::shutdown()
	{
		m_visible = true;
		for (auto *entity : children())
		{
			entity->setVisible(true);
		}
		GameEntity::shutdown();
	}


	void EditorEntity::update()
	{
		GameEntity::update();

		drawUI();
	}
}