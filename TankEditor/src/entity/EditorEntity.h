#pragma once
#include <scene/GameEntity.h>


namespace Tank
{
	/// <summary>
	/// A GameEntity which becomes invisible on startup, and visible on shutdown.
	/// Applies the same behaviour to first-generation children.
	/// 
	/// @todo Toggling enable/disable of a parent may break this system.
	/// </summary>
	class EditorEntity : public GameEntity
	{
	protected:
		EditorEntity(entt::entity handle, Scene *ecs) : GameEntity(handle, ecs) {}
		EditorEntity(const GameEntity &other) : GameEntity(other) {};
	public:
		virtual ~EditorEntity() = default;

		virtual void drawUI() = 0;
		virtual void startup() override;
		virtual void shutdown() override;
		virtual void update() override;
	};
}