#pragma once
#include <array>
#include "ECS.h"
#include "EntityComponentMap.h"

class BaseComponentManager
{
public:
	BaseComponentManager() = default;
	virtual ~BaseComponentManager() = default;
	BaseComponentManager(const BaseComponentManager&) = default;
	BaseComponentManager(BaseComponentManager&&) = default;
	BaseComponentManager& operator=(const BaseComponentManager&) = default;
	BaseComponentManager& operator=(BaseComponentManager&&) = default;
};

template <typename ComponentType>
class ComponentManager : public BaseComponentManager
{
private:
	size_t m_Size = 0;
	std::array<ComponentType, MAX_ENTITIES> m_Data{};
	EntityComponentMap m_ECMap;

public:
	ComponentType& AddComponent(Entity entity, const ComponentType& component)
	{
		m_Data[m_Size] = component;

		m_ECMap.AddEntity(entity);

		return m_Data[m_Size++];
	}

	void DestroyComponent(Entity entity)
	{
		m_Data[m_ECMap.ToArrIndex(entity)] = std::move(m_Data[m_Size]);

		m_ECMap.RemoveEntity();

		m_Size--;
	}

	ComponentType& GetComponent(Entity entity)
	{
		size_t arrIndex = m_ECMap.ToArrIndex(entity);

		return m_Data[arrIndex];
	}

	ComponentType* Data()
	{
		return m_Data.data();
	}

	size_t Size()
	{
		return m_Size;
	}
};