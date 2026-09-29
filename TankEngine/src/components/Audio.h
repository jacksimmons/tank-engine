#pragma once
#include <audio/AudioEngine.h>
#include <serialisation/Serialisation.h>


namespace Tank
{
	struct AudioComponent
	{
	private:
		ma_sound m_currentSound;
		bool m_hasSound = false;
	public:
		Resource AudioPath { "audio/test.wav", true };

		AudioComponent() = default;
		AudioComponent(const Res &audioPath) : AudioPath(audioPath) {};
		~AudioComponent();

		void updateSound();
		void play();
	};


	template <>
	json serialise<AudioComponent>(AudioComponent *);
	template <>
	void deserialise<AudioComponent>(const json &, AudioComponent *);
}