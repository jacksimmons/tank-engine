#pragma once


namespace Tank
{
	class Entity;
	class Scene;


	class EntitySerialisation
	{
	public:
		static void serialise(json &outData, const Scene &scene, const Entity &entity);
		static void deserialise(const json &data, Scene &scene, GameEntity &outEntity, GameEntity *parent = nullptr);
	};
}