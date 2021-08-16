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

		signature.reset();
		
	}

	m_EntityVisibility.set();
	m_EntityActivity.set();
}

Basic::Entity Basic::EntityManager::ICreateEntity()
{
	assert(m_EntitiesCount < MAX_ENTITIES && "Too many entities.");

	Basic::Entity entity = m_AvailableEntites.top();
	m_AvailableEntites.pop();
	m_EntitiesCount++;
	return entity;
}

void Basic::EntityManager::IDestroyEntity(Entity entity)
{
	assert(m_EntitiesCount < MAX_ENTITIES && "Entity out of range.");

	m_AvailableEntites.push(entity);
	m_EntitiesCount--;
}

Basic::Entity Basic::EntityManager::IEntitiesCount()
{
	return m_EntitiesCount;
}

Basic::Signature& Basic::EntityManager::IEntitySignature(Entity entity)
{
	assert(m_EntitiesCount < MAX_ENTITIES && "Entity out of range.");
	return m_EntitySignatures[entity];
}

bool Basic::EntityManager::IGetVisibililty(Entity entity)
{
	return m_EntityVisibility[entity];
}

void Basic::EntityManager::ISetVisibililty(Entity entity, bool visibility)
{
	m_EntityVisibility[entity] = visibility;
}

bool Basic::EntityManager::IGetActivity(Entity entity)
{
	return m_EntityVisibility[entity];
}

void Basic::EntityManager::ISetActivity(Entity entity, bool activity)
{
	m_EntityVisibility[entity] = activity;
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

bool Basic::EntityManager::GetVisibililty(Entity entity)
{
	return s_Instance.IGetVisibililty(entity);
}

void Basic::EntityManager::SetVisibililty(Entity entity, bool visibility)
{
	s_Instance.ISetVisibililty(entity, visibility);
}

bool Basic::EntityManager::GetActivity(Entity entity)
{
	return s_Instance.IGetActivity(entity);
}

void Basic::EntityManager::SetActivity(Entity entity, bool activity)
{
	s_Instance.ISetActivity(entity, activity);
}
