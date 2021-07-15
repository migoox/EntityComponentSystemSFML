#include "EntityManager.h"

EntityManager EntityManager::s_Instance;

EntityManager::EntityManager()
	: m_EntitiesCount(0)
{
	for (Entity i = 0; i < MAX_ENTITIES; i++)
	{
		m_AvailableEntitiesIDs.push(i);
	}
}

Entity EntityManager::ICreate()
{
	Entity newEntity = m_AvailableEntitiesIDs.front();
	m_AvailableEntitiesIDs.pop();
	m_EntitiesCount++;
	
	return newEntity;
}

void EntityManager::IDestroy(Entity entity)
{
	m_AvailableEntitiesIDs.push(entity);
	m_EntitiesCount--;
}

Entity EntityManager::Create()
{
	return Instance().ICreate();
}

void EntityManager::Destroy(Entity entity)
{
	Instance().IDestroy(entity);
}

EntityManager& EntityManager::Instance()
{
	return s_Instance;
}
