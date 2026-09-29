#include <Log.h>
#include <components/Camera.h>
#include <components/Light.h>
#include <components/Sprite.h>
#include <components/Collider.h>
#include <rendering/Renderer2D.h>
#include "Scene.h"
#include "Entity.h"


namespace Tank
{
	Scene *Scene::s_activeScene = nullptr;


	Scene::Scene(const std::string &name)
	{
		m_activeCamera = nullptr;
	}


	Entity Scene::createEntity()
	{
		Entity entity = { m_registry.create(), this };

		// ALL entities have these components.
		entity.addComponent<TransformComponent>();

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
			glm::vec3 centre = m_activeCamera->Camera.getTransformedCentre();
			glm::vec3 eye = m_activeCamera->getTransformedEye();
			glm::vec3 up = m_activeCamera->getTransformedUp();
			m_activeCamera->View = glm::lookAt(eye, centre, up);
		}

		// Handle collisions
		{
			auto view = m_registry.view<ColliderComponent>();

			for (auto e : view)
			{
				Entity entity = { e, this };
				const auto &collider = entity.getComponent<ColliderComponent>();

				// Check for collisions against all colliders
				for (auto o : view)
				{
					Entity other = { o, this };
					const auto &transform = other.getComponent<TransformComponent>();

					// Ignore self collisions
					if (other == entity) continue;

					if (collider.Shape->contains(transform.Translation))
					{
						TE_CORE_INFO(std::format("Collision: Offender {}, Recipient {}", entity.name(), other.name()));
					}
				}
			}
		}

		// Draw sprites
		{
			auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group)
			{
				SpriteRenderer::
			}
		}




		// Update camera, then the scene
		m_activeCamera->update();
	}


	unsigned Scene::addLight(LightComponent *light)
	{
		auto it = std::find(m_lights.begin(), m_lights.end(), light);
		if (it != m_lights.end())
		{
			TE_CORE_WARN("Light has already been added to this scene.");
		}

		m_lights.push_back(light);
		return m_lights.size() - 1;
	}


	void Scene::removeLight(LightComponent *light)
	{
		auto it = std::find(m_lights.begin(), m_lights.end(), light);
		if (it != m_lights.end())
		{
			m_lights.erase(it);
		}
		else
		{
			TE_CORE_WARN("Light was not found in this scene.");
		}
	}


	unsigned Scene::getNumLights(LightType type) const
	{
		unsigned cnt = 0;

		switch (type)
		{
		case LightType::Any:
			cnt = m_lights.size();
			break;
		default:
			for (LightComponent *light : m_lights)
			{
				if (light->getType() == type) cnt++;
			}
			break;
		}

		return cnt;
	}


	void Scene::onNodeDeleted(Entity *deleted) noexcept
	{
		(void)deleted;
	}


	// =======================
	//		Serialisation
	// =======================
	template <>
	json serialise<Scene>(Scene *in)
	{
		json serialised;
		serialised["isActiveScene"] = Scene::getActiveScene() == in;
		return serialised;
	}

	template <>
	void deserialise<Scene>(const json &serialised, Scene *out)
	{
		if (serialised["isActiveScene"]) Scene::setActiveScene(out);
	}
}