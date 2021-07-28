#pragma once
#include "ECSModules.h"
#include "EntityManager.h"

namespace Basic {
	class World;

	class GameObject
	{
	private:
		Entity m_Entity = 0;
		World* m_WorldPtr = nullptr;
		Transform* m_TransformPtr = nullptr;

	public:
		GameObject() = default;
		GameObject(Entity entity, World* worldPtr) : m_Entity(entity), m_WorldPtr(worldPtr), m_TransformPtr(nullptr) { }
		GameObject(Entity entity, World* worldPtr, Transform* trans) : m_Entity(entity), m_WorldPtr(worldPtr), m_TransformPtr(trans) { }

		bool IsVisible() const
		{
			return EntityManager::GetVisibililty(m_Entity);
		}

		void SetVisibility(bool visibility)
		{
			EntityManager::SetVisibililty(m_Entity, visibility);
		}

		bool IsActive() const
		{
			return EntityManager::GetActivity(m_Entity);
		}

		void SetActivity(bool activity)
		{
			EntityManager::SetActivity(m_Entity, activity);
		}

		bool operator<(const GameObject& rhs) const noexcept
		{
			return m_Entity < rhs.m_Entity;
		}

		bool operator==(const GameObject& rhs) const noexcept
		{
			return m_Entity == rhs.m_Entity;
		}

		bool operator!=(const GameObject& rhs) const noexcept
		{
			return m_Entity != rhs.m_Entity;
		}

		Signature GetSignature() const
		{
			return EntityManager::EntitySignature(m_Entity);
		}

		Entity GetEntity() const
		{
			return m_Entity;
		}

		// functions underneath are defined in World.h under World class

		void Destroy();

		Transform& GetTransform() const;

		template <typename ComponentType>
		ComponentType& AddComponent(ComponentType&& component);

		template <typename ComponentType>
		void RemoveComponent();

		template <typename ComponentType>
		ComponentType& GetComponent();

		template <typename ComponentType>
		bool HasComponent();
	};
} // end of Basic