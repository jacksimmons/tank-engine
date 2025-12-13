#pragma once
#include <utils/getset.hpp>


namespace Tank
{
	using TimePoint = std::chrono::system_clock::time_point;

	class TANK_API Time
	{
		friend class Application;
	private:
		static TimePoint s_startingTime;
		static TimePoint s_currentTime;

		static float s_frameDelta;

		static void setFrameDelta(std::clock_t lastFrameStart, std::clock_t lastFrameEnd)
		{
			s_frameDelta = (lastFrameEnd - lastFrameStart) / (float)CLOCKS_PER_SEC;
		}
	public:
		static Get<TimePoint> CurrentTime;
		static Get<float> FrameDelta;
	};
}