#include <Log.h>
#include <scene/Entity.h>
#include <scene/Scene.h>
#include <scene/SceneCamera.h>
#include <components/Components.h>
#include "EntitySerialisation.h"
#include "Serialisation.h"
#include "GlmSerialisation.h"


namespace Tank
{
	void EntitySerialisation::serialise(json &outData, const Scene &scene, const Entity &entity)
	{
		json &componentData = outData["components"];
		
		TE_CORE_ASSERT(entity.hasComponent<TransformComponent>(), "Serialise error: Missing essential Transform component");
		{
			auto &transform = entity.getComponent<TransformComponent>();
			componentData["Transform"] = {
				{ "rotation", quat::serialise(transform.rotation) },
				{ "scale", vec3::serialise(transform.scale) },
				{ "translation", vec3::serialise(transform.translation) },
			};
		}

		TE_CORE_ASSERT(entity.hasComponent<TreeComponent>(), "Serialise error: Missing essential Tree component");
		{
			auto &tree = entity.getComponent<TreeComponent>();
			json &treeData = componentData["Tree"];

			// Serialise all children - this will take a while.
			std::vector<json> children;
			for (auto &child : tree)
			{
				json childData;
				serialise(childData, scene, *child.get());
				treeData["children"].push_back(childData);
			}
		}

		if (entity.hasComponent<CameraComponent>())
		{
			auto &camera = entity.getComponent<CameraComponent>();
			json &cameraData = componentData["Camera"];

			cameraData["camera"] = Serialisation::serialise<SceneCamera>(&camera.camera);
			cameraData["panSpeed"] = camera.panSpeed;
			cameraData["rotationSpeed"] = camera.rotationSpeed;
		}

		if (entity.hasComponent<DirectionalLightComponent>())
		{
			auto &directionalLight = entity.getComponent<DirectionalLightComponent>();

			outData["DirectionalLight"] =
			{
				{ "intensity", Serialisation::serialise(&directionalLight.intensity) },
				{ "direction", vec3::serialise(directionalLight.direction) }
			};
		}

		if (entity.hasComponent<PointLightComponent>())
		{
			auto &pointLight = entity.getComponent<PointLightComponent>();

			outData["PointLight"] =
			{
				{ "intensity", Serialisation::serialise(&pointLight.intensity) },
			};
		}

		if (entity.hasComponent<SpriteComponent>())
		{
			auto &sprite = entity.getComponent<SpriteComponent>();
			json &spriteData = componentData["Sprite"];

			spriteData["texPath"] = Res::encode(sprite.getTexPath());
			spriteData["shader"] = Shader::serialise(sprite.shader);
		}

		if (entity.hasComponent<ModelComponent>())
		{
			auto &model = entity.getComponent<ModelComponent>();
			json &modelData = componentData["Model"];
			
			modelData["modelPath"] = Resource::encode(model.getModelPath());
			modelData["shader"] = Shader::serialise(model.shader);
			modelData["cullFace"] = model.cullFace;
		}

		if (entity.hasComponent<CubeMapComponent>())
		{
			auto &cubeMap = entity.getComponent<CubeMapComponent>();
			json &cubeMapData = componentData["CubeMap"];

			std::vector<std::string> encodedPaths;
			for (const Res &res : cubeMap.getTexPaths())
			{
				encodedPaths.push_back(Res::encode(res));
			}
			cubeMapData["texPaths"] = encodedPaths;

			cubeMapData["shader"] = Shader::serialise(cubeMap.shader);
		}

		if (entity.hasComponent<AudioComponent>())
		{
			auto &audio = entity.getComponent<AudioComponent>();

			outData["AudioComponent"] =
			{
				{ "audioPath", audio.audioPath.encode() }
			};
		}
	}

	void EntitySerialisation::deserialise(const json &data, Scene &scene, GameEntity &outEntity, GameEntity *parent)
	{
		TE_CORE_ASSERT(data.contains("components"), "Deserialise error: No component data");
		const json &componentData = data["components"];

		TE_CORE_ASSERT(componentData.contains("Transform"), "Deserialise error: Missing essential Transform component");
		{
			json transformData = componentData["Transform"];
			
			TransformComponent &transform = outEntity.addComponent<TransformComponent>();
			transform.rotation = quat::deserialise(transformData["rotation"]);
			transform.scale = vec3::deserialise(transformData["scale"]);
			transform.translation = vec3::deserialise(transformData["translation"]);
		}

		TE_CORE_ASSERT(componentData.contains("Tree"), "Deserialise error: Missing essential Tree component");
		{
			json treeData = componentData["Tree"];

			TreeComponent &tree = outEntity.addComponent<TreeComponent>(&outEntity, parent);

			for (const json &child : treeData["children"].get<std::vector<json>>())
			{
				auto childEntity = scene.createEntity();
				deserialise(child, scene, *childEntity, &outEntity);
				tree.addChild(std::move(childEntity));
			}
		}

		if (componentData.contains("Camera"))
		{
			json cameraData = componentData["Camera"];

			CameraComponent &camera = outEntity.addComponent<CameraComponent>();
			camera.camera = Serialisation::deserialise<SceneCamera>(cameraData["camera"]);
			camera.panSpeed = cameraData["panSpeed"];
			camera.rotationSpeed = cameraData["rotationSpeed"];
		}

		if (componentData.contains("DirectionalLight"))
		{
			json dirLightData = componentData["DirectionalLight"];

			DirectionalLightComponent &dirLight = outEntity.addComponent<DirectionalLightComponent>();
			dirLight.intensity = Serialisation::deserialise<LightIntensity>(dirLightData["intensity"]);
			dirLight.direction = vec3::deserialise(dirLightData["direction"]);
		}

		if (componentData.contains("PointLight"))
		{
			json ptLightData = componentData["PointLight"];

			PointLightComponent &ptLight = outEntity.addComponent<PointLightComponent>();
			ptLight.intensity = Serialisation::deserialise<LightIntensity>(ptLightData["intensity"]);
		}

		if (componentData.contains("Sprite"))
		{
			json spriteData = componentData["Sprite"];

			SpriteComponent &sprite = outEntity.addComponent<SpriteComponent>();
			sprite.shader = Shader{ {}, ShaderSources::deserialise(spriteData["shader"]) };
			sprite.setTexPath(Res::decode(spriteData["texPath"]));
		}

		if (componentData.contains("Model"))
		{
			json modelData = componentData["Model"];

			ModelComponent &model = outEntity.addComponent<ModelComponent>();
			model.shader = Shader{ {}, ShaderSources::deserialise(modelData["shader"]) };
			model.setModelPath(Resource::decode(modelData["modelPath"]));
			model.cullFace = modelData["cullFace"];

			model.process();
		}

		if (componentData.contains("CubeMap"))
		{
			json cubeMapData = componentData["CubeMap"];

			CubeMapComponent &cubeMap = outEntity.addComponent<CubeMapComponent>();
			cubeMap.shader = { {}, ShaderSources::deserialise(cubeMapData["shader"]) };

			std::array<Res, 6> decodedPaths;
			for (int i = 0; i < 6; i++)
			{
				decodedPaths[i] = Res::decode(cubeMapData["texPaths"][i]);
			}

			cubeMap.setTexPaths(decodedPaths);
		}

		if (componentData.contains("Audio"))
		{
			json audioData = data["Audio"];

			outEntity.addComponent<AudioComponent>(Res::decode(audioData["audioPath"]));
		}
	}
}