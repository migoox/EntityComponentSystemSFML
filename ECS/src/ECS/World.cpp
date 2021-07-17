#include "World.h"
#include "EntityHandle.h"


void World::Init()
{
	for (auto& system : m_Systems)
	{
		system->Init();
	}
}

void World::Update(const sf::Time& deltaTime)
{
	for (auto& system : m_Systems)
	{
		system->Update(deltaTime);
	}
}

void World::Render(sf::RenderTarget& target)
{
	for (auto& system : m_Systems)
	{
		system->Render(target);
	}
}

void World::AddSystem(std::shared_ptr<System> system)
{
	system->RegisterWorld(this);

	m_Systems.push_back(system);
}

EntityHandle World::CreateEntity()
{
	return { EntityManager::CreateEntity(), this };
}

void World::DestroyEntity(Entity entity)
{
	for (auto& system : m_Systems)
	{
		system->TryToUnregisterEntity(entity);
	}

	EntityManager::DestroyEntity(entity);
}
