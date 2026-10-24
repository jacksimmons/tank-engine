#include <scene/Scene.h>
#include <components/Components.h>
#include "SceneSerialisation.h"
#include "EntitySerialisation.h"


namespace Tank
{
	void SceneSerialisation::serialise(json &outData, Scene &scene)
	{
		outData["isActiveScene"] = Scene::getActiveScene() == &scene;

		// Handle all entity serialisations
		json &entityData = outData["entities"];

		for (const auto &e : scene.m_registry.view<TransformComponent>())
		{
			GameEntity entity = { e, &scene };
			EntitySerialisation::serialise(entityData, scene, entity);
		}
	}
}