#pragma once
#include <memory>
#include "System.h"
#include "GameObject.h"

namespace Basic {

	class World
	{
	private:
		std::vector<std::shared_ptr<System>> m_Systems;

		std::unique_ptr<ComponentManager> m_ComponentManager;

		std::unique_ptr<EventBus> m_EventBus;

	public:
		World()
		{
			m_ComponentManager = std::make_unique<ComponentManager>();
			m_EventBus = std::make_unique<EventBus>();
		}

		void InitSystems();

		void Update(const sf::Time& deltaTime);

		void Render(sf::RenderTarget& target);


		ComponentManager* GetComponentManager()
		{
			return m_ComponentManager.get();
		}
		EventBus* GetEventBus()
		{
			return m_EventBus.get();
		}

		// System
		void AddSystem(std::shared_ptr<System> system);

		// Entity Manager Mask
		GameObject CreateEntity()
		{
			Entity entity = EntityManager::CreateEntity();

			// transform component is always added
			GameObject gameObject(entity, this, &AddComponent<Transform>(entity, Transform()));

			return gameObject;
		}

		void DestroyEntity(Entity entity);

		GameObject GetGameObject(Entity entity)
		{
			return GameObject(entity, this, &GetComponent<Transform>(entity));
		}

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
		ComponentType& AddComponent(Entity entity, ComponentType&& component)
		{
			ComponentType& result = m_ComponentManager->AddComponent<ComponentType>(entity, std::move(component));

			for (auto& system : m_Systems)
			{
				system->TryToRegisterEntity(entity);
			}

			return result;
		}

		template <typename ComponentType>
		void RemoveComponent(Entity entity)
		{
			if (!m_ComponentManager->IsRemovableComponent<ComponentType>())
				return;

			m_ComponentManager->RemoveComponent<ComponentType>(entity);

			for (auto& system : m_Systems)
			{
				system->UpdateRegistration(entity);
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

		template <typename ComponentType>
		bool EntityHasThisComponent(Entity entity)
		{
			return m_ComponentManager->EntityHasThisComponent<ComponentType>(entity);
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

	inline Transform& GameObject::GetTransform() const 
	{
		if (m_TransformPtr == nullptr)
			return m_WorldPtr->GetComponent<Transform>(m_Entity);
		else
			return *m_TransformPtr;
	}

	template <typename ComponentType>
	inline ComponentType& GameObject::AddComponent(ComponentType&& component)
	{
		return m_WorldPtr->AddComponent<ComponentType>(m_Entity, std::move(component));
	}

	template <typename ComponentType>
	inline void GameObject::RemoveComponent()
	{
		m_WorldPtr->RemoveComponent<ComponentType>();
	}

	template <typename ComponentType>
	inline ComponentType& GameObject::GetComponent()
	{
		return m_WorldPtr->GetComponent<ComponentType>(m_Entity);
	}

	template<typename ComponentType>
	inline bool GameObject::HasComponent()
	{
		return m_WorldPtr->EntityHasThisComponent<ComponentType>(m_Entity);
	}

	inline void GameObject::Destroy()
	{
		m_WorldPtr->DestroyEntity(m_Entity);
	}
} // end of Basic