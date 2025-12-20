#pragma once
#include "miniaudio/miniaudio.h"


namespace Tank
{
	class TANK_API AudioEngine final
	{
	private:
		ma_engine_config m_engineConfig;
		ma_engine m_engine;
		static std::unique_ptr<AudioEngine> s_instance;

		AudioEngine();
	public:
		~AudioEngine();

		static AudioEngine &instance();
		static ma_engine *ma_engine() { return &instance().m_engine; }
		static bool handleResult(ma_result result, const std::string &errorMsg);
	};
}