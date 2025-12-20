#pragma once
#include "node.hpp"
#include <audio_engine.hpp>


namespace Tank
{
	class TANK_API Audio : public Node
	{
	public:
		virtual json serialise() override;
		virtual void deserialise(const json &) override;

	private:
		fs::path m_audioPath;
		ma_sound m_currentSound;
		bool m_hasSound = false;

	public:
		Audio(const std::string &name = "Audio", const fs::path &audioPath = "audio/test.wav");
		~Audio();

		void setAudioPath(const fs::path &path) { m_audioPath = path; updateSound(); };
		const fs::path &getAudioPath() const { return m_audioPath; };
		void updateSound();
		void play();
	};
}