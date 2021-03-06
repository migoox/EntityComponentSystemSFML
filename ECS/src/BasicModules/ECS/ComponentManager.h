#pragma once
#include "../Transform.h"
#include "ComponentArray.h"
#include <memory>
#include <unordered_map>

namespace Basic
{
	class ComponentManager
	{
	private:
		std::unordered_map<const char*, ComponentID> m_ComponentIDs;

		std::unordered_map<const char*, std::shared_ptr<BaseComponentArray>> m_ComponentArrays;

		ComponentID m_NextComponentID = 0;

	private:
		template <typename ComponentType>
		void RegisterComponent()
		{
			const char* name = typeid(ComponentType).name();
			// id assignment
			m_ComponentIDs[name] = m_NextComponentID;

			// creating array
			m_ComponentArrays[name] = std::make_shared<ComponentArray<ComponentType>>();
			m_ComponentArrays[name]->ComponentID() = m_NextComponentID;

			m_NextComponentID++;
		}

	public:
		template <typename ComponentType>
		bool IsRemovableComponent()
		{
			std::string name = typeid(ComponentType).name();
			if (name == "class sf::Transformable")
				return false;
			return true;
		}

		template <typename ComponentType>
		void TryToRegisterComponent()
		{
			const char* name = typeid(ComponentType).name();

			if (m_ComponentIDs.find(name) == m_ComponentIDs.end())
			{
				RegisterComponent<ComponentType>();
			}
		}

		template <typename ComponentType>
		std::shared_ptr<ComponentArray<ComponentType>> GetComponentArray()
		{
			const char* name = typeid(ComponentType).name();

			return std::static_pointer_cast<ComponentArray<ComponentType>>(m_ComponentArrays[name]);
		}

		template <typename ComponentType>
		ComponentID GetComponentID()
		{
			const char* name = typeid(ComponentType).name();

			return m_ComponentIDs[name];
		}

		template <typename ComponentType>
		ComponentType& AddComponent(Entity entity, ComponentType&& component)
		{
			const char* name = typeid(ComponentType).name();

			TryToRegisterComponent<ComponentType>();

			ComponentType& result = GetComponentArray<ComponentType>()->AddComponent(entity, std::move(component));		

			EntityManager::EntitySignature(entity).set(m_ComponentIDs[name]);

			return result;
		}

		template <typename ComponentType>
		void RemoveComponent(Entity entity)
		{
			if (!IsRemovableComponent<ComponentType>())
				return;

			const char* name = typeid(ComponentType).name();

			EntityManager::EntitySignature(entity).reset(m_ComponentIDs[name]);

			GetComponentArray<ComponentType>()->RemoveComponent(entity);
		}

		template <typename ComponentType>
		ComponentType& GetComponent(Entity entity)
		{
			return GetComponentArray<ComponentType>()->GetComponent(entity);
		}

		template <typename ComponentType>
		ComponentType* GetComponentPtr(Entity entity)
		{
			return &GetComponentArray<ComponentType>()->GetComponent(entity);
		}

		template <typename ComponentType>
		Entity GetEntityByComponentArrayIndex(size_t index)
		{
			return GetComponentArray<ComponentType>()->GetEntityByComponentArrayIndex(index);
		}

		template <typename ComponentType>
		ComponentType* ArrayData()
		{
			TryToRegisterComponent<ComponentType>();
			return GetComponentArray<ComponentType>()->Data();
		}

		template <typename ComponentType>
		size_t Size()
		{
			TryToRegisterComponent<ComponentType>();
			return GetComponentArray<ComponentType>()->Size();
		}

		void EntityDestroyed(Entity entity)
		{
			for (auto mapElement : m_ComponentArrays)
			{
				mapElement.second->EntityDestroyed(entity);

				Signature& signature = EntityManager::EntitySignature(entity);

				signature.reset();
			}
		}

		template <typename ComponentType>
		bool EntityHasThisComponent(Entity entity)
		{
			const char* name = typeid(ComponentType).name();

			auto iter = m_ComponentArrays.find(name);

			if (iter != m_ComponentArrays.end())
			{
				return iter->second->EntityHasThisComponent(entity);
			}
			
			return false;
		}
	};
} // end of Basic