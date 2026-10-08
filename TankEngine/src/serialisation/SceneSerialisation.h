#pragma once


namespace Tank
{
	class Entity;
	class Scene;


	class SceneSerialisation
	{
	public:
		static void serialise(json &outData, Scene &scene);
		static void deserialise(const json &data, Scene &outScene);
	};
}