#include <log.hpp>
#include "audio_engine.hpp"


namespace Tank
{
	AudioEngine::AudioEngine()
	{
		ma_result result;
		result = ma_engine_init(NULL, &m_engine);
		if (result != MA_SUCCESS)
		{
			TE_CORE_CRITICAL(std::format("Failed to init audio engine, code {}", (int)result));
			return;
		}

		ma_engine_play_sound(&m_engine, "audio/test.wav", NULL);
	}
}