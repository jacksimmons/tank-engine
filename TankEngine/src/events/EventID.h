#pragma once



namespace Tank
{
	enum EventID
	{
		TANK_EVENT_ENTITY_ADOPTED = 1 << 0,
		TANK_EVENT_ENTITY_DISOWNED = 1 << 1,
		
		// The first event which is permitted for use by client applications.
		// Very likely to change between builds.
		#define TANK_CLIENT_EVENT 1 << 2
	};
}
