#include "World.h"
#include "GameObject.h"


void Basic::World::Init()
{
	for (auto& system : m_Systems)
	{
		system->Init();
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

Basic::GameObject Basic::World::CreateEntity()
{
	GameObject gameObject = { EntityManager::CreateEntity(), this };

	// transform component is always added
	gameObject.AddComponent<Transform>(Transform());

	return gameObject;
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

Basic::GameObject Basic::World::GetGameObject(Entity entity)
{
	return {entity, this};
}

void Basic::System::RegisterWorld(Basic::World* world)
{
	m_ParentWorld = world;
	m_ComponentManager = world->GetComponentManagerPtr();
}