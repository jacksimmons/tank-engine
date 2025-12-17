#include <log.hpp>
#include "audio.hpp"


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
	ma_engine Audio::s_engine;
	ma_engine_config Audio::s_engineConfig;
	bool Audio::s_engineStarted;

	// Public, read-only reference to s_engine.
	Get<ma_engine> Audio::Engine = Get<ma_engine>(
		// Will reference Audio::s_engine
		Audio::s_engine,
		// Getter for s_engine
		[](const ma_engine &engine) -> const ma_engine &
		{
			if (s_engineStarted)
			{
				return engine;
			}

			s_engineConfig = ma_engine_config_init();
			s_engineConfig.dataCallback = data_callback;

			ma_result result;
			result = ma_engine_init(&s_engineConfig, &s_engine);
			if (result != MA_SUCCESS)
			{
				TE_CORE_CRITICAL(std::format("Failed to init audio engine, code {}", (int)result));
			}
			s_engineStarted = true;
			return engine;
		}
	);


	//
	// =-=-=-=-= Serialisation =-=-=-=-=
	//
	void Audio::deserialise(const json &serialised)
	{
		m_audioPath = fs::path(std::string(serialised["audioPath"]));

		Node::deserialise(serialised);
	}

	json Audio::serialise()
	{
		json serialised = Node::serialise();
		serialised["audioPath"] = m_audioPath.string();

		return serialised;
	}


	//
	// =-=-=-=-= Instance =-=-=-=-=
	//
	Audio::Audio(const std::string &name, const fs::path &audioPath)
		: Node(name)
	{
		m_type = "Audio";
		m_audioPath = audioPath;
		TE_CORE_INFO(m_audioPath.string());
		TE_CORE_INFO(AudioPath().string());
	}

	void Audio::play()
	{
		ma_result result = ma_engine_play_sound(&s_engine, "audio/test.wav", NULL);
	}
}