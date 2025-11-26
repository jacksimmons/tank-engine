#pragma once


namespace Tank
{
	class TANK_API Time
	{
	private:
		static float s_frameDelta;
	public:
		static void setFrameDelta(std::clock_t lastFrameStart, std::clock_t lastFrameEnd)
		{
			s_frameDelta = (lastFrameEnd - lastFrameStart) / (float)CLOCKS_PER_SEC;
		}
		static float getFrameDelta()
		{
			return s_frameDelta;
		}
	};
}