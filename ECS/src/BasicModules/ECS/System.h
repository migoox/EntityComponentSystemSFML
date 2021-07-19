#pragma once
#include <set>
#include <SFML/Graphics.hpp>

#include "EntityManager.h"
#include "ComponentManager.h"


namespace Basic {
	class World;

	enum class SignatureType
	{
		Inclusive, // (entity's signature & system's signature) == system's signature
		Constitutive // exactly the same signatures
	};

	class System
	{
	private:
		SignatureType m_SignatureType;

	protected:
		std::set<Entity> m_Entities;

		Signature m_Signature;

		World* m_ParentWorld;

		ComponentManager* m_ComponentManager;

	protected:
		template <typename ComponentType>
		void AddToSignature();

		template <typename ComponentType>
		void RemoveFromSignature();

	public:
		System()
		{
			m_ComponentManager = nullptr;
			m_ParentWorld = nullptr;
			m_SignatureType = SignatureType::Inclusive;
		}
		virtual ~System() = default;
		System(const System&) = default;
		System& operator=(const System&) = default;
		System(System&&) = default;
		System& operator=(System&&) = default;

		void SetSignatureType(SignatureType type)
		{
			if (m_Entities.size() == 0)
				m_SignatureType = type;
		}

		virtual void Init() {};

		virtual void Update(const sf::Time& deltaTime) {};

		virtual void Render(sf::RenderTarget& target) {};

		void RegisterWorld(World* world);

		void TryToRegisterEntity(Entity entity)
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
					m_Entities.insert(entity);
			}
			else
			{
				/*
					Entity contains the same components as system
				*/
				if (m_Signature == entitySignature)
					m_Entities.insert(entity);
			}
		}

		void TryToUnregisterEntity(Entity entity)
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
	};
} // end of Basic