#pragma once

#include "core.hpp"

namespace Tank
{
	class TANK_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void run() = 0;
	};

	std::unique_ptr<Tank::Application> createApplication();
}