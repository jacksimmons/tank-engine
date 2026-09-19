#pragma once


namespace Tank
{
	/// @brief An object which can be serialised to a type T.
	template <typename T = json>
	class TANK_API ISerialisable
	{
	public:
		virtual T serialise() = 0;
		virtual void deserialise(const T &serialised) = 0;
	};
}