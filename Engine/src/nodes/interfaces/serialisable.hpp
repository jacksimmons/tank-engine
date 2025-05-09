#pragma once


namespace Tank
{
	class TANK_API ISerialisable
	{
	public:
		virtual json serialise() = 0;
	};
}