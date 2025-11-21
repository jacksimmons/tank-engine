#pragma once
#include "engine.hpp"


namespace Tank
{
	class Node;
	class Scene;
	class KeyInput;

	class Player : public Application
	{
	private:
		std::unique_ptr<Node> m_root;
	public:
		Player();
		~Player();
		void loadDemoScene();
	protected:
		virtual void step() override;
		virtual void handleKeyInput() override;
	};
}