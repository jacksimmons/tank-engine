#pragma once
#include <scripting/Script.h>
#include <KeyInput.h>
#include "Entity.h"


namespace Tank
{
	class NameComponent;
	class TransformComponent;
	class TreeComponent;
	class KeyInput;


	/// @brief Implementation for a practical Entity in a scene.
	class TANK_API GameEntity : public Entity
	{
	private:
		std::unique_ptr<KeyInput> m_keyInput;
		std::vector<std::unique_ptr<Script>> m_scripts;

		/// @brief Whether or not user scripts have started running.
		bool m_started = false;
	public:
		GameEntity(entt::entity handle, Scene *ecs) : Entity(handle, ecs) {}
		GameEntity(const GameEntity &other) : Entity(other) {};
		~GameEntity() = default;

		const std::string &name() const;
		void setName(const std::string &name) noexcept;

		KeyInput *keyInput() const;
		TransformComponent &transform() const;
		TreeComponent &tree() const;

		virtual void startup();
		virtual void shutdown();
		virtual void preupdate();
		virtual void update();
		virtual void destroy();

		void addScript(std::unique_ptr<Script>);
		bool removeScript(const Res &path);

		std::vector<Res> getScriptPaths();
	};


	template <>
	json serialise<GameEntity>(GameEntity *);
	template <>
	GameEntity deserialise(const json &);
}