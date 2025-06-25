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
		loadDemoScene();
		Scene::setActiveScene(dynamic_cast<Tank::Scene*>(m_root.get()));
	}


	void Player::loadDemoScene()
	{
		// Create nodes
		{
			auto scene = std::make_unique<Tank::Scene>();
			{
				auto camera = std::make_unique<Tank::Camera>();
				scene->setActiveCamera(dynamic_cast<Tank::Camera*>(camera.get()));
				scene->addChild(std::move(camera));
			}
			{
				ShaderSources sources;
				sources.vertex.location = "skybox.vert";
				sources.fragment.location = "skybox.frag";
				scene->addChild(std::make_unique<Tank::CubeMap>("CubeMap", sources));
			}
			{
				ShaderSources sources;
				sources.vertex.location = "shader.vert";
				sources.fragment.location = "shader.frag";

				auto object = std::unique_ptr<Tank::Model>(new Model("Doom", sources, fs::current_path() / "models/doom/doom_E1M1.obj"));
				object->getTransform()->setLocalTranslation({ 0, 0, 0 });
				scene->addChild(std::move(object));

				auto backpackPhysics = std::unique_ptr<Tank::PhysicsBody>(new PhysicsBody("BackpackBody", 1e15f));
				auto backpack = std::unique_ptr<Tank::Model>(new Model("Backpack", sources, fs::current_path() / "models/backpack/backpack.obj"));
				backpack->getTransform()->setLocalScale({ 100, 100, 100 });
				backpackPhysics->getTransform()->setLocalTranslation({ 0, 0, 200 });
				backpackPhysics->addChild(std::move(backpack));
				scene->addChild(std::move(backpackPhysics));

				auto spritePhysics = std::unique_ptr<Tank::PhysicsBody>(new PhysicsBody("SpriteBody", 1e15f));
				auto sprite = std::unique_ptr<Tank::Sprite>(new Sprite("Sprite", sources, fs::current_path() / "textures/awesomeface.png"));
				spritePhysics->addChild(std::move(sprite));
				scene->addChild(std::move(spritePhysics));
			}

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


	void Player::handleKeyInput()
	{
	}


	std::unique_ptr<Application> createApplication()
	{
		return std::make_unique<Player>();
	}
}
