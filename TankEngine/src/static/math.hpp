#pragma once


namespace Tank
{
	namespace MathF
	{
		TANK_API float min(float a, float b)
		{
			if (a < b) return a;
			return b;
		}

		TANK_API float max(float a, float b)
		{
			if (a > b) return a;
			return b;
		}
	}
}