#pragma once
#include "Event.h"

namespace Basic {
	class BaseListeners
	{
	public:
		virtual ~BaseListeners() = default;

		virtual void Call(Event* ev) = 0;
	};

	template <typename EventType>
	class Listeners : public BaseListeners
	{
	private:
		std::list<std::function<void(EventType* ev)>> m_Listeners;

	public:

		void Call(Event* ev)
		{
			for (auto func : m_Listeners)
			{
				func(static_cast<EventType*>(ev));
			}
		}

		void Add(std::function<void(EventType* ev)> listener)
		{
			m_Listeners.push_back(listener);
		}
	};
}