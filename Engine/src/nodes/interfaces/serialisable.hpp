#pragma once


namespace Tank
{
	/// <summary>
	/// An object which can be serialised.
	/// </summary>
	class TANK_API ISerialisable
	{
	public:
		virtual json serialise() = 0;
	};
}