#pragma once
#include "engine.hpp"


class ImGuiIO;
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
	protected:
		virtual void step() override;
		virtual void beginImGui(ImGuiIO &) override {};
		virtual void endImGui() override {};
	};
}