#pragma once
#include <set>
#include <iostream>

#include "EntityManager.h"
#include "SFML/Graphics.hpp"

namespace Basic {
	class World;

	enum class SignatureType
	{
		Inclusive,
		Constitutive
	};

	class System
	{
	private:
		SignatureType m_SignatureType;

	protected:
		std::set<Entity> m_Entities;

		Signature m_Signature;

		World* m_ParentWorld;

	protected:
		template <typename ComponentType>
		void AddToSignature();

		template <typename ComponentType>
		void RemoveFromSignature();

	public:
		System()
		{
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

		virtual void Init() = 0;

		virtual void Update(const sf::Time& deltaTime) = 0;

		virtual void Render(sf::RenderTarget& target) = 0;

		void RegisterWorld(World* world)
		{
			m_ParentWorld = world;
		}

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

				bool match = true;
				for (size_t i = 0; i < MAX_COMPONENTS; i++)
				{
					if (m_Signature[i] == 1)
					{
						if (entitySignature[i] != 1)
						{
							match = false;
							break;
						}
					}
				}
				if (match)
				{
					m_Entities.insert(entity);
				}
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
				std::cout << "unregister\n";
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