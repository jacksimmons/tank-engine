#pragma once


namespace Tank
{
	struct NameComponent
	{
		std::string Name;


		NameComponent(const std::string &name = "Entity") : Name(name) {}
	};
}