#pragma once


extern std::unique_ptr<Tank::Application> Tank::createApplication();


int main(int argc, char **argv)
{
	// Memory scope encapsulation
	// *For memory debugging.
	{
		// Initialise logger
		Tank::Log::init();
		TE_CORE_INFO("Initialised Log.");

		std::unique_ptr<Tank::Application> app = Tank::createApplication();
		app->run();

		spdlog::shutdown();
	}
}
