#include "time.hpp"
#include <chrono>


namespace Tank
{
	// Initialise the starting time of the application.
	TimePoint Time::s_startingTime = std::chrono::system_clock::now();

	// Initialise the current time to the starting time.
	// Add a public getter which updates the current time.
	TimePoint Time::s_currentTime = Time::s_startingTime;
	Get<TimePoint> Time::CurrentTime = Get<TimePoint>(
		Time::s_currentTime,
		[](auto) -> const TimePoint &
		{
			Time::s_currentTime = std::chrono::system_clock::now();
			return Time::s_currentTime;
		}
	);

	// Set time between frames to 0 until the first frame has ended.
	float Time::s_frameDelta = 0;
	Get<float> Time::FrameDelta = Time::s_frameDelta;
}