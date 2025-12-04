#include <log.hpp>
#include "event_manager.hpp"


namespace Tank
{
	std::unordered_map<std::string, std::unique_ptr<BaseEvent>> EventManager::s_eventMap;


	void EventManager::addEvent(const std::string &name, BaseEvent *event)
	{
		if (s_eventMap.contains(name))
		{
			TE_CORE_ERROR(std::format("Key already exists: {}", name));
		}

		s_eventMap[name] = std::unique_ptr<BaseEvent>(event);
	}


	void EventManager::removeEvent(const std::string &name)
	{
		if (!s_eventMap.contains(name))
		{
			TE_CORE_ERROR(std::format("Key doesn't exist: {}", name));
		}

		s_eventMap.erase(name);
	}
}