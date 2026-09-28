#include "Time.h"
#include <chrono>


namespace Tank
{
	// Initialise the current time.
	TimePoint Time::s_currentTime = std::chrono::system_clock::now();
	// Set time between frames to 0 until the first frame has ended.
	float Time::s_frameDelta = 0;


	const TimePoint &Time::getCurrentTime()
	{
		Time::s_currentTime = std::chrono::system_clock::now();
		return Time::s_currentTime;
	}
}