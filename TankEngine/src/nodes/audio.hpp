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
		Resource m_audioPath;
		ma_sound m_currentSound;
		bool m_hasSound = false;

	public:
		Audio(
			const std::string &name = "Audio",
			const Resource &audioPath = Resource("audio/test.wav", true)
		);
		~Audio();

		void setAudioPath(const Resource &path) { m_audioPath = path; };
		const Resource &getAudioPath() const { return m_audioPath; };

		void updateSound();
		void play();
	};
}