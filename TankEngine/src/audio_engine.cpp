#include "audio_engine.hpp"
#include "log.hpp"


//
// =-=-=-=-= Callback =-=-=-=-=
//
static void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
}


namespace Tank
{
	//
	// =-=-=-=-= Static =-=-=-=-=
	//
	std::unique_ptr<AudioEngine> AudioEngine::s_instance;


	AudioEngine &AudioEngine::instance()
	{
		if (!s_instance)
		{
			TE_CORE_INFO("Creating AudioEngine");
			s_instance = std::unique_ptr<AudioEngine>(new AudioEngine());
		}
		return *(s_instance.get());
	}


	bool AudioEngine::handleResult(ma_result result, const std::string &errorMsg)
	{
		if (result != MA_SUCCESS)
		{
			TE_CORE_ERROR(errorMsg);
			return false;
		}
		return true;
	}


	//
	// =-=-=-=-= Instance =-=-=-=-=
	//
	AudioEngine::AudioEngine()
	{
		m_engineConfig = ma_engine_config_init();
		//s_engineConfig.dataCallback = data_callback;

		ma_result result;
		result = ma_engine_init(&m_engineConfig, &m_engine);
		if (!handleResult(result, std::format("Failed to initialise audio engine, code {}", (int)result))) return;

		result = ma_engine_start(&m_engine);
		if (!handleResult(result, std::format("Failed to start audio engine, code {}", (int)result))) return;

		TE_CORE_INFO("Successfully initialised & started audio engine.");
	}


	AudioEngine::~AudioEngine()
	{
		ma_engine_uninit(&m_engine);
	}
}