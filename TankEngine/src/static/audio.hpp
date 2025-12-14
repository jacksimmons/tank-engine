#pragma once
#include <thread>


namespace Tank
{
	class TANK_API Audio
	{
	public:
		static void play()
		{
			system(std::format("{} audio/Test.opus", "ffplay.exe -nodisp").c_str());
		}
	};
}