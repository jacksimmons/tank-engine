#pragma once


namespace Tank
{
	class Project;
}

namespace Tank::Export
{
	/// @brief Exports a project to be playable as a standalone application.
	/// Requires TankPlayer to be built.
	TANK_API bool project(Project *, const fs::path &);
}