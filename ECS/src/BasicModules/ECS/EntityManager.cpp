#include "EntityManager.h"

Basic::EntityManager Basic::EntityManager::s_Instance;

Basic::EntityManager::EntityManager()
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

Basic::Entity Basic::EntityManager::ICreateEntity()
{
	Basic::Entity entity = m_AvailableEntites.top();
	m_AvailableEntites.pop();
	m_EntitiesCount++;
	return entity;
}

void Basic::EntityManager::IDestroyEntity(Entity entity)
{
	m_AvailableEntites.push(entity);
	m_EntitiesCount--;
}

Basic::Entity Basic::EntityManager::IEntitiesCount()
{
	return m_EntitiesCount;
}

Basic::Signature& Basic::EntityManager::IEntitySignature(Entity entity)
{
	return m_EntitySignatures[entity];
}

Basic::Entity Basic::EntityManager::CreateEntity()
{
	return s_Instance.ICreateEntity();
}

void Basic::EntityManager::DestroyEntity(Entity entity)
{
	s_Instance.IDestroyEntity(entity);
}

Basic::Entity Basic::EntityManager::EntitiesCount()
{
	return s_Instance.IEntitiesCount();
}

Basic::Signature& Basic::EntityManager::EntitySignature(Entity entity)
{
	return s_Instance.IEntitySignature(entity);
}
