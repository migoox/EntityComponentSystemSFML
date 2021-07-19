#include "EntityMap.h"


void Basic::EntityMap::AddEntity(Entity entity)
{
	m_EntityToArrayIndexMap[entity] = m_Size;
	m_ArrayIndexToEntityMap[m_Size] = entity;

	m_Size++;
}

void Basic::EntityMap::RemoveEntity(Entity removedEntity)
{
	size_t removedArrIndex = m_EntityToArrayIndexMap[removedEntity];

	size_t lastArrIndex = m_Size - 1;
	Entity lastEntity = m_ArrayIndexToEntityMap[lastArrIndex];

	if (m_EntityToArrayIndexMap[removedEntity] != lastArrIndex)
	{
		m_ArrayIndexToEntityMap[removedArrIndex] = lastEntity;
		m_EntityToArrayIndexMap[lastEntity] = removedArrIndex;
	}

	m_ArrayIndexToEntityMap.erase(lastArrIndex);
	m_EntityToArrayIndexMap.erase(removedEntity);

	m_Size--;
}

Basic::Entity Basic::EntityMap::ToEntity(size_t arrayIndex)
{
	return m_ArrayIndexToEntityMap[arrayIndex];
}

size_t Basic::EntityMap::ToArrIndex(Entity entity)
{
	return m_EntityToArrayIndexMap[entity];
}

bool Basic::EntityMap::IsContain(Entity entity)
{
	return m_EntityToArrayIndexMap.find(entity) != m_EntityToArrayIndexMap.end();
}

void Basic::EntityMap::Clear()
{
	m_ArrayIndexToEntityMap.clear();
	m_EntityToArrayIndexMap.clear();
	m_Size = 0;
}