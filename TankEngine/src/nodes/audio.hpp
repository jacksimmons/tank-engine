#pragma once
#include "node.hpp"
#include "miniaudio/miniaudio.h"


namespace Tank
{
	class TANK_API Audio : public Node
	{
	public:
		static Get<ma_engine> Engine;

		virtual json serialise() override;
		virtual void deserialise(const json &) override;

	private:
		static bool s_engineStarted;
		static ma_engine_config s_engineConfig;
		static ma_engine s_engine;

		fs::path m_audioPath;

	public:
		Audio(const std::string &name = "Audio", const fs::path &audioPath = "audio/test.wav");

		Get<fs::path> AudioPath = m_audioPath;
		void play();
	};
}