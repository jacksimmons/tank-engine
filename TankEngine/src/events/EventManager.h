#pragma once
#include <Log.h>
#include "Event.h"


namespace Tank
{
	class TANK_API EventManager
	{
	private:
		static std::unordered_map<unsigned, std::unique_ptr<BaseEvent>> s_eventMap;
	public:
		static void addEvent(unsigned id, BaseEvent *event);
		static void removeEvent(unsigned id);

		template <class ...TArgs>
		static Event<TArgs...> *getEvent(unsigned id)
		{
			if (!s_eventMap.contains(id))
			{
				TE_CORE_ERROR(std::format("No matching entry for key {}", id));
				return nullptr;
			}

			if (auto *e = dynamic_cast<Event<TArgs...> *>(s_eventMap[id].get()))
			{
				return e;
			}
			else
			{
				TE_CORE_ERROR(std::format("Failed to cast with provided type for key {}", id));
				return nullptr;
			}
		}
	
		/// @brief Invoke Event by ID. You should always pass an enum into this function, and never
		/// reuse event IDs.
		template <class ...TArgs>
		static void invokeEvent(unsigned id, TArgs... args)
		{
			Event<TArgs...> *event = EventManager::getEvent<TArgs...>(id);
			event->invoke(args...);
		}
	};
}