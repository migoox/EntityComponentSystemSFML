#pragma once
#include "EntityManager.h"
#include "EntityMap.h"

#include <iostream>
#include <functional>

namespace Basic {
	class BaseComponentArray
	{
	private:
		size_t m_ComponentID = 0;

	public:
		virtual ~BaseComponentArray() = default;
		virtual void EntityDestroyed(Entity entity) = 0;

		size_t& ComponentID()
		{
			return m_ComponentID;
		}

		bool EntityHasThisComponent(Entity entity)
		{
			Signature entitySignature = EntityManager::EntitySignature(entity);
			Signature componentSignature;
			componentSignature.set(m_ComponentID);

			return componentSignature == (componentSignature & entitySignature);
		}
	};

	template <typename ComponentType>
	class ComponentArray : public BaseComponentArray
	{
	private:
		std::array<ComponentType, MAX_ENTITIES> m_ComponentArray;

		EntityMap m_ECMap;

		size_t m_Size = 0;

	public:
		ComponentType& AddComponent(Entity entity, ComponentType&& component)
		{
			// check if entity is not assigned
			if (EntityHasThisComponent(entity))
			{
				return m_ComponentArray[m_ECMap.ToArrIndex(entity)];
			}

			// copying component to array
			m_ComponentArray[m_Size] = std::move(component);

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
			if (EntityHasThisComponent(entity))
			{
				RemoveComponent(entity);
			}
		}

		Entity GetEntityByComponentArrayIndex(size_t index)
		{
			return m_ECMap.ToEntity(index);
		}

		ComponentType* Data()
		{
			if (m_Size == 0)
				return nullptr;
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