#include <Log.h>
#include "Audio.h"


namespace Tank
{
	AudioComponent::~AudioComponent()
	{
		if (m_hasSound) ma_sound_uninit(&m_currentSound);
		m_hasSound = false;
	}


	void AudioComponent::updateSound()
	{
		if (m_hasSound) ma_sound_uninit(&m_currentSound);

		ma_result result = ma_sound_init_from_file(AudioEngine::maEngine(), audioPath.resolvePathStr().c_str(), 0, NULL, NULL, &m_currentSound);
		if (!AudioEngine::handleResult(result, std::format("Failed to update sound with result {}. File: {}", (int)result, audioPath.resolvePathStr()))) return;

		m_hasSound = true;
		TE_CORE_INFO("Successfully updated sound to " + audioPath.resolvePathStr());
	}


	void AudioComponent::play()
	{
		if (!m_hasSound) updateSound();

		ma_result result = ma_sound_start(&m_currentSound);
		if (!AudioEngine::handleResult(result, std::format("Failed to play sound with result {}. File: {}", (int)result, audioPath.resolvePathStr()))) return;

		TE_CORE_INFO("Successfully played sound " + audioPath.resolvePathStr());
	}
}
