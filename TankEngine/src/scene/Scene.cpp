#include <Log.h>
#include <static/Time.h>
#include <components/Camera.h>
#include <components/Light.h>
#include <components/Collider.h>
#include <components/PhysicsBody.h>
#include <components/CubeMap.h>
#include <components/Model.h>
#include <components/Sprite.h>
#include <physics/Physics.h>
#include <rendering/Renderer.h>
#include <rendering/Renderer2D.h>
#include <rendering/Lighting.h>
#include <Shader.h>
#include "Scene.h"
#include "GameEntity.h"


namespace Tank
{
	Scene *Scene::s_activeScene = nullptr;


	Scene::Scene(bool isActive)
	{
		m_activeCamera = nullptr;

		if (isActive) Scene::setActiveScene(this);
		//scene->setActiveCamera((Camera *)scene->childFromTree(serialised["activeCam"]));
	}


	std::unique_ptr<GameEntity> Scene::createEntity()
	{
		auto entity = std::make_unique<GameEntity>(m_registry.create(), this);

		// ALL entities have these components.
		entity->addComponent<NameComponent>();
		entity->addComponent<TransformComponent>();
		entity->addComponent<TreeComponent>();

		return entity;
	}



	void Scene::update()
	{
		// Ignore updates if no camera is active.
		if (!m_activeCamera) return;

		// Update camera view
		/// <summary>
		/// 1. Rotate center around the eye("Universe rotates around camera")
		/// (Translate center by - eye, then rotate it around origin with R)
		/// (Then translate the new center by eye)
		/// 2. Translate the eye with the displacement matrix(Camera movement)
		/// 3. Rotate the up vector around origin with R.
		/// Combining these gives the view matrix.
		/// </summary>
		{
			glm::vec3 centre = m_activeCamera->camera.getTransformedCentre();
			glm::vec3 eye = m_activeCamera->camera.getTransformedEye();
			glm::vec3 up = m_activeCamera->camera.getTransformedUp();
			m_activeCamera->camera.m_view = glm::lookAt(eye, centre, up);
		}

		// Collisions
		{
			auto view = m_registry.view<ColliderComponent>();

			for (auto e : view)
			{
				GameEntity entity = { e, this };
				const auto &collider = entity.getComponent<ColliderComponent>();

				// Check for collisions against all colliders
				for (auto o : view)
				{
					// Ignore interactions with self
					if (e == o) continue;

					GameEntity other = { o, this };
					const auto &transform = other.getComponent<TransformComponent>();

					if (collider.shape->contains(transform.translation))
					{
						TE_CORE_INFO(std::format("Collision: Offender {}, Recipient {}", entity.name(), other.name()));
					}
				}
			}
		}

		// Physics
		{
			auto view = m_registry.view<PhysicsBodyComponent>();

			for (auto e : view)
			{
				Entity entity = { e, this };

				// Ensure velocities list is correct size before starting
				float dt = Time::getFrameDelta();

				// Handle this body's gravity, applied to all other physics bodies
				for (auto o : view)
				{
					// Ignore interactions with self
					if (e == o) continue;

					Entity other = { o, this };
					const auto &M = entity.getComponent<PhysicsBodyComponent>();
					const auto &m = other.getComponent<PhysicsBodyComponent>();

					Physics::handleGravity(M, m, dt);
				}
			}
		}

		// Rendering (CubeMaps)
		{
			auto view = m_registry.view<CubeMapComponent>();

			for (auto e : view)
			{
				Entity entity = { e, this };
				
				// Don't render components on invisible entities
				if (!entity.isVisible()) continue;

				auto &cubeMap = entity.getComponent<CubeMapComponent>();
				Renderer::drawCubeMap(&cubeMap, m_activeCamera->camera);
			}
		}

		// Rendering (Models)
		{
			auto group = m_registry.group<TransformComponent>(entt::get<ModelComponent>);
			for (auto e : group)
			{
				auto [transform, model] = group.get<TransformComponent, ModelComponent>(e);
				Entity entity = { e, this };

				// Don't render components on invisible entities
				if (!entity.isVisible()) continue;

				// Apply lighting
				const Shader &shader = model.shader;
				shader.use();
				applyLightsToShader(shader);
				shader.unuse();

				Renderer::drawModel(transform, model, m_activeCamera->camera);
			}
		}

		// Rendering (Sprites)
		{
			auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto e : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(e);
				Entity entity = { e, this };

				// Don't render components on invisible entities
				if (!entity.isVisible()) continue;

				// Apply lighting
				const Shader &shader = sprite.shader;
				shader.use();
				applyLightsToShader(shader);
				shader.unuse();

				Renderer2D::drawSprite(transform, sprite, m_activeCamera->camera);
			}
		}
	}


	void Scene::applyLightsToShader(const Shader &shader)
	{
		// Directional Lights
		{
			auto view = m_registry.view<DirectionalLightComponent>();
			int index = -1;
			for (auto e : view)
			{
				index++;
				if (index >= LIGHT::DIRECTIONAL_ARRAY_SIZE)
				{
					TE_CORE_ERROR(std::format("Directional light limit ({}) exceeded!", LIGHT::DIRECTIONAL_ARRAY_SIZE));
					break;
				}

				auto [directionalLight] = view.get(e);
				Lighting::applyDirectionalLightToShader(directionalLight, shader, index);
			}

			shader.setInt("num_dir_lights", index + 1);
		}

		// Point Lights
		{
			auto view = m_registry.view<PointLightComponent>();
			int index = -1;
			for (auto e : view)
			{
				index++;
				if (index >= LIGHT::POINT_ARRAY_SIZE)
				{
					TE_CORE_ERROR(std::format("Point light limit ({}) exceeded!", LIGHT::POINT_ARRAY_SIZE));
					break;
				}


				auto [pointLight] = view.get(e);
				Lighting::applyPointLightToShader(pointLight, shader, index);
			}

			shader.setInt("num_point_lights", index + 1);
		}
	}


	void Scene::onNodeDeleted(Entity *deleted) noexcept
	{
		(void)deleted;
	}


	// =======================
	//		Serialisation
	// =======================
	Scene *loadScene(const std::filesystem::path &scenePath)
	{
		std::string sceneFile;
		if (File::readLines(scenePath, sceneFile) != File::ReadResult::Success)
		{
			TE_CORE_ERROR(std::format("Failed to deserialise from file {}", scenePath.string()));
			return nullptr;
		}

		json serialised;
		try
		{
			serialised = json::parse(sceneFile);
		}
		catch (std::exception e)
		{
			TE_CORE_ERROR(std::format("Couldn't parse {} into JSON.", scenePath.string()));
			return nullptr;
		}

		if (Scene *scene = ReflectionRegistry::deserialise<Scene>(serialised))
		{
			return scene;
		}

		TE_CORE_ERROR("File selected was not a valid scene.");
		return nullptr;
	}


	void saveScene(Scene *scene, const std::filesystem::path &scenePath)
	{
		std::string sceneFile;
		if (File::readLines(scenePath, sceneFile) == File::ReadResult::Error)
		{
			TE_CORE_ERROR(std::format("Failed to serialise to file {}", scenePath.string()));
		}

		// Write with pretty print (indent=4)
		TE_CORE_INFO(std::format("Saving scene to {}", scenePath.string()));

		std::string dump = serialise(scene).dump(4);
		std::ofstream out(scenePath);

		if (!out)
		{
			TE_CORE_ERROR("Failed to serialise scene at file {}", scenePath.string());
			return;
		}

		out.write(dump.c_str(), dump.size());
	}


	template <>
	json serialise<Scene>(Scene *in)
	{
		json serialised;
		serialised["isActiveScene"] = Scene::getActiveScene() == in;
		return serialised;
	}

	template <>
	Scene deserialise(const json &serialised)
	{
		Scene scene { serialised["isActiveScene"] };
		return scene;
	}
}