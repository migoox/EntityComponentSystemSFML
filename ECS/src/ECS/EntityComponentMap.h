#pragma once
#include <map>
#include "ECS.h"

class EntityComponentMap
{
private:
	std::map<Entity, size_t> m_EntityToArrayIndexMap{};
	std::map<size_t, Entity> m_ArrayIndexToEntityMap{};

	size_t m_Size = 0;

public:
	EntityComponentMap() = default;

	void AddEntity(Entity entity);
	void RemoveEntity(Entity entity);

	Entity ToEntity(size_t arrayIndex);
	size_t ToArrIndex(Entity entity);

	void Clear();
};