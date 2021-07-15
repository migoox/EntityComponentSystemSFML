#include "EntityComponentMap.h"

void EntityComponentMap::AddEntity(Entity entity)
{
	m_EntityToArrayIndexMap[entity] = m_Size;
	m_ArrayIndexToEntityMap[m_Size] = entity;

	m_Size++;
}

void EntityComponentMap::RemoveEntity(Entity removedEntity)
{
	size_t removedArrIndex = m_EntityToArrayIndexMap[removedEntity];

	size_t lastArrIndex = m_Size - 1;
	Entity lastEntity = m_EntityToArrayIndexMap[lastArrIndex];

	m_ArrayIndexToEntityMap[removedArrIndex] = lastEntity;
	m_EntityToArrayIndexMap[lastEntity] = removedArrIndex;

	m_ArrayIndexToEntityMap.erase(lastArrIndex);
	m_ArrayIndexToEntityMap.erase(removedEntity);

	m_Size--;
}

Entity EntityComponentMap::ToEntity(size_t arrayIndex)
{
	return m_ArrayIndexToEntityMap[arrayIndex];
}

size_t EntityComponentMap::ToArrIndex(Entity entity)
{
	return m_EntityToArrayIndexMap[entity];
}

void EntityComponentMap::Clear()
{
	m_ArrayIndexToEntityMap.clear();
	m_EntityToArrayIndexMap.clear();
	m_Size = 0;
}
