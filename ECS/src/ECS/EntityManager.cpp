#include "EntityManager.h"

EntityManager EntityManager::s_Instance;

EntityManager::EntityManager()
{
	for (size_t entity = MAX_ENTITIES; entity > 0; entity--)
	{
		m_AvailableEntites.push(entity - 1);
	}

	for (size_t entity = MAX_ENTITIES; entity > 0; entity--)
	{
		Signature& signature = m_EntitySignatures[entity - 1];

		for (size_t i = 0; i < MAX_COMPONENTS; i++)
		{
			signature[i] = 0;
		}
	}
}

Entity EntityManager::ICreateEntity()
{
	Entity entity = m_AvailableEntites.top();
	m_AvailableEntites.pop();
	m_EntitiesCount++;
	return entity;
}

void EntityManager::IDestroyEntity(Entity entity)
{
	m_AvailableEntites.push(entity);
	m_EntitiesCount--;
}

Entity EntityManager::IEntitiesCount()
{
	return m_EntitiesCount;
}

Signature& EntityManager::IEntitySignature(Entity entity)
{
	return m_EntitySignatures[entity];
}

Entity EntityManager::CreateEntity()
{
	return s_Instance.ICreateEntity();
}

void EntityManager::DestroyEntity(Entity entity)
{
	s_Instance.IDestroyEntity(entity);
}

Entity EntityManager::EntitiesCount()
{
	return s_Instance.IEntitiesCount();
}

Signature& EntityManager::EntitySignature(Entity entity)
{
	return s_Instance.IEntitySignature(entity);
}
