#include <Log.h>
#include "EventManager.h"


namespace Tank
{
	std::unordered_map<unsigned, std::unique_ptr<BaseEvent>> EventManager::s_eventMap;


	void EventManager::addEvent(unsigned id, BaseEvent *event)
	{
		if (s_eventMap.contains(id))
		{
			TE_CORE_ERROR(std::format("Key already exists: {}", id));
		}

		s_eventMap[id] = std::unique_ptr<BaseEvent>(event);
	}


	void EventManager::removeEvent(unsigned id)
	{
		if (!s_eventMap.contains(id))
		{
			TE_CORE_ERROR(std::format("Key doesn't exist: {}", id));
		}

		s_eventMap.erase(id);
	}
}