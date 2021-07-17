#pragma once
#include <memory>

#include "System.h"
#include "ComponentManager.h"

struct EntityHandle;

class World
{
private:
	std::vector<std::shared_ptr<System>> m_Systems;

	std::unique_ptr<ComponentManager> m_ComponentManager;

public:
	World()
	{
		m_ComponentManager = std::make_unique<ComponentManager>();
	}

	void Init();

	void Update(const sf::Time& deltaTime);

	void Render(sf::RenderTarget& target);

	// System
	void AddSystem(std::shared_ptr<System> system);


	// Entity Manager Mask
	EntityHandle CreateEntity();

	void DestroyEntity(Entity entity);

	// Component Manager Mask
	template <typename ComponentType>
	void TryToRegisterComponent()
	{
		m_ComponentManager->TryToRegisterComponent<ComponentType>();
	}

	template <typename ComponentType>
	ComponentID GetComponentID()
	{
		return m_ComponentManager->GetComponentID<ComponentType>();
	}

	template <typename ComponentType>
	ComponentType& AddComponent(Entity entity, const ComponentType& component)
	{
		ComponentType& result = m_ComponentManager->AddComponent<ComponentType>(entity, component);

		for (auto& system : m_Systems)
		{
			system->TryToRegisterEntity(entity);
		}

		return result;
	}

	template <typename ComponentType>
	void RemoveComponent(Entity entity)
	{
		m_ComponentManager->RemoveComponent<ComponentType>(entity);

		for (auto& system : m_Systems)
		{
			system->TryToRegisterEntity(entity);
		}
	}

	template <typename ComponentType>
	ComponentType& GetComponent(Entity entity)
	{
		return m_ComponentManager->GetComponentArray<ComponentType>()->GetComponent(entity);
	}

	template <typename ComponentType>
	ComponentType* ArrayData()
	{
		return m_ComponentManager->GetComponentArray<ComponentType>()->Data();
	}
};

template<typename ComponentType>
inline void System::AddToSignature()
{
	m_ParentWorld->TryToRegisterComponent<ComponentType>();

	ComponentID id = m_ParentWorld->GetComponentID<ComponentType>();
	m_Signature.set(id);
}

template<typename ComponentType>
inline void System::RemoveFromSignature()
{
	ComponentID id = m_ParentWorld->GetComponentID<ComponentType>();
	m_Signature.reset(id);
}