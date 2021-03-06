#include "World.h"


void Basic::World::InitSystems()
{
	for (auto& system : m_Systems)
	{
		system->Init();
	}

	for (auto& system : m_Systems)
	{
		system->OnStart();
	}
}

void Basic::World::Update(const sf::Time& deltaTime)
{
	for (auto& system : m_Systems)
	{
		system->Update(deltaTime);
	}
}

void Basic::World::Render(sf::RenderTarget& target)
{
	for (auto& system : m_Systems)
	{
		system->Render(target);
	}
}

void Basic::World::AddSystem(std::shared_ptr<System> system)
{
	system->RegisterWorld(this);

	m_Systems.push_back(system);
}

void Basic::World::DestroyEntity(Entity entity)
{
	for (auto& system : m_Systems)
	{
		system->TryToUnregisterEntity(entity);
	}
	
	m_ComponentManager->EntityDestroyed(entity);

	EntityManager::DestroyEntity(entity);
}

void Basic::System::RegisterWorld(Basic::World* world)
{
	m_ParentWorld = world;
	m_ComponentManager = world->GetComponentManager();
	m_EventBus = world->GetEventBus();
}

Basic::GameObject& Basic::System::Instantiate(GameObject& gameObject)
{
	gameObject = m_ParentWorld->CreateEntity();
	return gameObject;
}

Basic::GameObject Basic::System::Instantiate()
{
	return m_ParentWorld->CreateEntity();
}

void Basic::System::Destroy(GameObject& gameObject)
{
	gameObject.Destroy();
}

