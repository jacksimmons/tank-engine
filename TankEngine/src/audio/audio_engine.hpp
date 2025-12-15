#pragma once
#include <miniaudio/miniaudio.h>


namespace Tank
{
	class TANK_API AudioEngine
	{
	private:
		ma_engine m_engine;
	public:
		AudioEngine();
	};
}