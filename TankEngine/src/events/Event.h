#pragma once


namespace Tank
{
	/// <summary>
	/// A function called with event data, when the event
	/// occurs.
	/// </summary>
	template <class ...TArgs>
	using EventHandler = std::function<void(TArgs...)>;


	class BaseEvent
	{
	public:
		virtual ~BaseEvent() = default;
	};


	template <class ...TArgs>
	class Event : public BaseEvent
	{
	protected:
		std::vector<EventHandler<TArgs...>> m_handlers;
	public:
		void registerHandler(EventHandler<TArgs...> handler) { m_handlers.push_back(handler); }
		void deregisterAllHandlers() { m_handlers.clear(); }

		void invoke(TArgs... args)
		{
			for (EventHandler handler : m_handlers)
			{
				handler(args...);
			}
		}
	};
}