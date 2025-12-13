#include "time.hpp"


namespace Tank
{
	// Set time between frames to 0 until the first frame has ended.
	float Time::s_frameDelta = 0;
	Get<float> Time::FrameDelta = Time::s_frameDelta;
}