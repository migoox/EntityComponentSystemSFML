#include "GameObject.h"


void Basic::World::InitSystems()
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
	m_ComponentManager = world->GetComponentManagerPtr();
}

Basic::GameObject Basic::World::CreateEntity()
{
	GameObject gameObject = { EntityManager::CreateEntity(), this };

	// transform component is always added
	gameObject.AddComponent<Transform>(Transform());

	return gameObject;
}

Basic::GameObject Basic::World::GetGameObject(Entity entity)
{
	return { entity, this };
}

void Basic::System::TryToRegisterEntity(Entity entity)
{
	Signature entitySignature = EntityManager::EntitySignature(entity);

	if (m_SignatureType == SignatureType::Inclusive)
	{
		/*
			ex. ENTITY contains Transform, RigidBody and Collider,
			SYSTEM has Transform and Collider
			System will add entity

			ex2. ENTITY contains Transform, RigidBody, Shape,
			SYSTEM has Transform and Collider
			System won't add entity
		*/

		if (m_Signature == (m_Signature & entitySignature)) // logical operator AND
		{
			m_Entities.insert(entity);
			GameObject go;
			go.ThisEntity = entity;
			go.WorldPtr = m_ParentWorld;
		}
	}
	else
	{
		/*
			Entity contains the same components as system
		*/
		if (m_Signature == entitySignature)
		{
			m_Entities.insert(entity);
			//m_GameObjects.insert({ entity, m_ParentWorld });
		}
	}
}

void Basic::System::TryToUnregisterEntity(Entity entity)
{
	if (m_Entities.size() > 0)
	{
		auto it = m_Entities.begin();
		for (auto& element : m_Entities)
		{
			if (element == entity)
				break;
			it++;
		}
		m_Entities.erase(it);
	}
}