#pragma once
#include "EntityManager.h"
#include "EntityComponentMap.h"

namespace Basic {
	class BaseComponentArray
	{
	public:
		virtual ~BaseComponentArray() = default;
		virtual void EntityDestroyed(Entity entity) = 0;
	};

	template <typename ComponentType>
	class ComponentArray : public BaseComponentArray
	{
	private:
		std::array<ComponentType, MAX_ENTITIES> m_ComponentArray;

		EntityComponentMap m_ECMap;

		size_t m_Size = 0;

	public:
		ComponentType& AddComponent(Entity entity, const ComponentType& component)
		{
			// copying component to array
			m_ComponentArray[m_Size] = component;

			// updating entity -> arrIndex and arrIndex -> entity system
			m_ECMap.AddEntity(entity);

			// returning component reference and increasing m_Size by one
			return m_ComponentArray[m_Size++];
		}

		void RemoveComponent(Entity entity)
		{
			if (m_Size > 1)
			{
				size_t arrIndex = m_ECMap.ToArrIndex(entity);
				m_ComponentArray[arrIndex] = m_ComponentArray[m_Size - 1];
			}
			m_ECMap.RemoveEntity(entity);

			m_Size--;
		}

		void EntityDestroyed(Entity entity) override
		{
			if (m_ECMap.IsContain(entity))
			{
				RemoveComponent(entity);
			}
		}

		ComponentType* Data()
		{
			return m_ComponentArray.data();
		}

		ComponentType& GetComponent(Entity entity)
		{
			size_t arrIndex = m_ECMap.ToArrIndex(entity);
			return m_ComponentArray[arrIndex];
		}

		size_t Size()
		{
			return m_Size;
		}
	};

} // end of basic