#pragma once
#include <list>
#include "../Colliders/ColliderItem.h"
#include "../../ECS/World.h"

// components
#include "../Components/Collider.h"

namespace Basic {
	class Solver 
	{
	private:
		World* m_ParentWorld;

	protected:
		Signature m_Signature;

	protected:
		template<typename ComponentType>
		void AddToSignature()
		{
			m_ParentWorld->TryToRegisterComponent<ComponentType>();

			ComponentID id = m_ParentWorld->GetComponentID<ComponentType>();
			m_Signature.set(id);
		}

		// it always will check in "inclusive" way
		bool HasCorrectSignature(const GameObject& gameObject)
		{
			return m_Signature == (m_Signature & gameObject.GetSignature());
		}

		virtual void SetSignature() = 0;

	public:
		Solver(World* world) : m_ParentWorld(world) { }

		virtual void Solve(std::list<CollisionInfo>& collisions, const sf::Time& deltaTime) = 0;
	};
}