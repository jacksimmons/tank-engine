#pragma once
#include <utils/getset.hpp>


namespace Tank
{
	class TANK_API Time
	{
		friend class Application;
	private:
		static float s_frameDelta;

		static void setFrameDelta(std::clock_t lastFrameStart, std::clock_t lastFrameEnd)
		{
			s_frameDelta = (lastFrameEnd - lastFrameStart) / (float)CLOCKS_PER_SEC;
		}
	public:
		static Get<float> FrameDelta;
	};
}