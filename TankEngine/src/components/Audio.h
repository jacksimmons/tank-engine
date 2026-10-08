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
		Resource audioPath { "audio/test.wav", true };

		AudioComponent() = default;
		AudioComponent(const Res &audioPath) : audioPath(audioPath) {};
		~AudioComponent();

		void updateSound();
		void play();
	};
}