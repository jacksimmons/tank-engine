#pragma once
#ifdef PLATFORM_WINDOWS
#include <iostream>
#include <memory>

extern std::unique_ptr<Tank::Application> Tank::createApplication();

int main(int argc, char **argv)
{
	Tank::Log::init();
	TE_CORE_WARN("Initialised Log.");
	TE_INFO("HI");

	auto app = Tank::createApplication();
	app->run();
}
#endif