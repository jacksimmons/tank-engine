#pragma once


namespace Tank
{
	class Entity;


	class EntitySerialisation
	{
	public:
		static void serialise(json &out, const Entity &entity);
		static Entity deserialise(const json &in);
	};
}