#pragma once
#include <set>
#include <deque>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "EntityManager.h"
#include "ComponentManager.h"
#include "GameObject.h"

namespace Basic {
	class System
	{
	protected: 
		// way of filtring entities
		enum class SignatureType
		{
			Inclusive, // (entity's signature & system's signature) == system's signature
			Constitutive // exactly the same signatures
		};

	protected:
		std::deque<GameObject> m_GameObjects;

		Signature m_Signature;

		World* m_ParentWorld;

		ComponentManager* m_ComponentManager;

	protected:
		// transform is added to system's signature by default

		// implementation in World.h since there is whole World class needed
		template <typename ComponentType>
		void AddToSignature();

		// implementation in World.h since there is whole World class needed
		template <typename ComponentType>
		void RemoveFromSignature();

	private:
		// it is not allowed to change signature type(way of filtring) directly
		SignatureType m_SignatureType;


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
			if (m_GameObjects.size() == 0)
				m_SignatureType = type;
		}

		virtual void Init() {};

		virtual void Update(const sf::Time& deltaTime) {};

		virtual void Render(sf::RenderTarget& target) {};

		void RegisterWorld(World* world);

		void TryToRegisterEntity(Entity entity);

		void UpdateRegistration(Entity entity);

		void TryToUnregisterEntity(Entity entity);
	};
} // end of Basic