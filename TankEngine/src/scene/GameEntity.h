#pragma once
#include "Entity.h"


namespace Tank
{
	/// @brief An entity which supports user-defined scripts.
	class TANK_API GameEntity : public Entity
	{
	private:
		std::unique_ptr<KeyInput> m_keyInput;
		std::vector<std::unique_ptr<Script>> m_scripts;

		/// @brief Whether or not user scripts have started running.
		bool m_started = false;
	public:
		GameEntity() = default;
		~GameEntity() = default;

		KeyInput *keyInput() const;

		virtual void startup() override;
		virtual void shutdown() override;
		virtual void update() override;

		void addScript(std::unique_ptr<Script>);
		bool removeScript(const Res &path);

		std::vector<Res> getScriptPaths();
	};


	template <>
	json serialise<GameEntity>(GameEntity *);
	template <>
	void deserialise<GameEntity>(const json &, GameEntity *);
}