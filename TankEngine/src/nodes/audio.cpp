#include <log.hpp>
#include "audio.hpp"


namespace Tank
{
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
		setAudioPath(audioPath);
	}


	Audio::~Audio()
	{
		if (m_hasSound) ma_sound_uninit(&m_currentSound);
		m_hasSound = false;
	}


	void Audio::updateSound()
	{
		if (m_hasSound) ma_sound_uninit(&m_currentSound);

		ma_result result = ma_sound_init_from_file(AudioEngine::maEngine(), m_audioPath.string().c_str(), 0, NULL, NULL, &m_currentSound);
		if (!AudioEngine::handleResult(result, std::format("Failed to update sound with result {}. File: {}", (int)result, m_audioPath.string()))) return;

		m_hasSound = true;
		TE_CORE_INFO("Successfully updated sound to " + m_audioPath.string());
	}

	void Audio::play()
	{
		ma_result result = ma_sound_start(&m_currentSound);
		if (!AudioEngine::handleResult(result, std::format("Failed to play sound with result {}. File: {}", (int)result, m_audioPath.string()))) return;

		TE_CORE_INFO("Successfully played sound " + m_audioPath.string());
	}
}