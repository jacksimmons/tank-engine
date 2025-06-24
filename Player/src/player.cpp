#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include "player.hpp"
#include "scene_serialisation.hpp"
#include "nodes/node.hpp"
#include "nodes/scene.hpp"
#include "nodes/ui/scene_view.hpp"

namespace Tank
{
	Player::Player() : Application(
		{
			GLFW_KEY_F1,
			GLFW_KEY_F2,
			GLFW_KEY_F3,
			GLFW_KEY_F4,
			GLFW_KEY_F5,
			GLFW_KEY_F6,

			GLFW_KEY_W,
			GLFW_KEY_A,
			GLFW_KEY_S,
			GLFW_KEY_D,
			GLFW_KEY_Q,
			GLFW_KEY_E,

			GLFW_KEY_I,
			GLFW_KEY_J,
			GLFW_KEY_K,
			GLFW_KEY_L,
			GLFW_KEY_U,
			GLFW_KEY_O,
		}
	)
	{
		// Set the ImGui context, over DLL boundary
		ImGui::SetCurrentContext(getContext());

		m_root = std::unique_ptr<Node>(Tank::Serialisation::loadScene("test.scene"));
		Scene::setActiveScene(dynamic_cast<Tank::Scene*>(m_root.get()));
		m_sceneView = std::make_unique<_SceneView>("SceneView", getWindowSize(), getWindowSize(), m_keyInput.get(), true);
	}


	Player::~Player()
	{
	}


	void Player::step()
	{
		m_sceneView->update();
	}


	void Player::handleKeyInput()
	{
	}


	std::unique_ptr<Tank::Application> createApplication()
	{
		return std::make_unique<Player>();
	}
}
