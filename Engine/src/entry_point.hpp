#pragma once

#include "stb_image.h"


extern std::unique_ptr<Tank::Application> Tank::createApplication();


int main(int argc, char **argv)
{
	// Memory scope encapsulation
	// *For memory debugging.
	{
		// Initialise logger
		Tank::Log::init();
		TE_CORE_INFO("Initialised Log.");

		// Flip loaded textures on the y-axis.
		stbi_set_flip_vertically_on_load(1);

		std::unique_ptr<Tank::Application> app = Tank::createApplication();
		app->run();

		spdlog::shutdown();
	}
}
