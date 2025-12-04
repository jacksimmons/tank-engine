#pragma once
#include "event.hpp"


namespace Tank
{
	class TANK_API EventManager
	{
	private:
		static std::unordered_map<std::string, std::unique_ptr<BaseEvent>> s_eventMap;
	public:
		static void addEvent(const std::string &id, BaseEvent *event);
		static void removeEvent(const std::string &id);

		template <class ...TArgs>
		static Event<TArgs...> *getEvent(const std::string &id)
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
			}
		}
	};
}