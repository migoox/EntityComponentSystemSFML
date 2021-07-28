#pragma once
#include <set>
#include <deque>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "EntityManager.h"
#include "ComponentManager.h"
#include "GameObject.h"
#include "../EventSystem/EventBus.h"

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
		std::vector<GameObject> m_GameObjects;

		Signature m_Signature;

		World* m_ParentWorld;

		EventBus* m_EventBus;

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
			m_EventBus = nullptr;
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

		// init is called for adding components to signatures and registering components
		// creating game objects here or interacting with components is not allowed
		// can be used for initialization of stuff not associated with engine
		virtual void Init() {};

		// every initialization which implies interaction with engine should be here
		virtual void OnStart() {};

		virtual void Update(const sf::Time& deltaTime) {};

		virtual void Render(sf::RenderTarget& target) {};

		void RegisterWorld(World* world);

		void TryToRegisterEntity(Entity entity);

		void UpdateRegistration(Entity entity);

		void TryToUnregisterEntity(Entity entity);

		GameObject& Instantiate(GameObject& gameObject);

		GameObject Instantiate();

		void Destroy(GameObject& gameObject);
	};
} // end of Basic