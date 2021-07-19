#pragma once
#include <list>
#include <map>
#include <functional>
#include <typeindex>

#include "Listeners.h"

namespace Basic {
	class EventBus
	{
	private:
		std::map<const char*, std::shared_ptr<BaseListeners>> m_ListenersMap;

		template <typename EventType>
		std::shared_ptr<Listeners<EventType>> GetListeners()
		{
			const char* name = typeid(EventType).name();
			return std::static_pointer_cast<Listeners<EventType>>(m_ListenersMap[name]);
		}

	public:
		template <typename EventType>
		void AddListener(const std::function<void(EventType* ev)>& func)
		{
			const char* name = typeid(EventType).name();

			if (m_ListenersMap.find(name) == m_ListenersMap.end())
			{
				m_ListenersMap[name] = std::make_shared<Listeners<EventType>>();
			}

			GetListeners<EventType>()->Add(func);
		}

		template <typename EventType>
		void Call(std::unique_ptr<Event> ev)
		{
			const char* name = typeid(EventType).name();

			m_ListenersMap[name]->Call(ev.get());
		}

		template <typename EventType>
		void Call(Event&& ev)
		{
			const char* name = typeid(EventType).name();

			m_ListenersMap[name]->Call(&ev);
		}
	};
}