#pragma once
#include <thread>


namespace Tank
{
	class TANK_API Audio
	{
	public:
		static void play()
		{
			system(std::format("{} audio/Test.opus", ".\\vendor\\ffmpeg\\bin\\ffplay.exe -nodisp").c_str());
		}
	};
}