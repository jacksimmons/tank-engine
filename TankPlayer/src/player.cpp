#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "player.hpp"
#include "scene_serialisation.hpp"
#include "nodes/node.hpp"
#include "nodes/scene.hpp"
#include "nodes/camera.hpp"
#include "nodes/cube_map.hpp"
#include "nodes/model.hpp"
#include "nodes/physics/physics_body.hpp"
#include "nodes/sprite.hpp"
#include "nodes/light.hpp"


namespace Tank
{
	Player::Player() : Application()
	{
		auto scene = std::unique_ptr<Tank::Scene>(Serialisation::loadScene("scene.json", m_factory.get()));
		Scene::setActiveScene(scene.get());
		m_root = std::move(scene);

		// Lights can only be added after scene load
		std::string name = "DirLight";
		auto light = std::make_unique<Tank::DirLight>(name,
			glm::vec3{ 0.0f, -1.0f, 0.0f },
			glm::vec3{ 0.02f, 0.02f, 0.02f },
			glm::vec3{ 0.2f, 0.2f, 0.2f },
			glm::vec3{ 0.1f, 0.1f, 0.1f }
		);
		Scene::getActiveScene()->addChild(std::move(light));
	}


	Player::~Player()
	{
		m_root->shutdown();
	}


	void Player::step()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00); // Don't write to stencil by default

		m_root->update();
		m_root->startup();
	}


	std::unique_ptr<Application> createApplication()
	{
		return std::make_unique<Player>();
	}
}
