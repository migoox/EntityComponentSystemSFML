#pragma once
#include <SFML/Graphics.hpp>
#include <list>

#include "StateMachine/State.h"
#include "ECS.h"


namespace Basic {
	class Scene : public State
	{
	private:
		std::list<World> m_Worlds;

	protected:
		World* m_CurrentWorld;

	public:
		Scene() 
		{ 
			m_Worlds.push_back(World()); 
			m_CurrentWorld = &m_Worlds.back();
		}

		void InitWorldSystems()
		{
			m_CurrentWorld->InitSystems();
		}

		GameObject& Instantiate(GameObject& gameObject)
		{
			gameObject = m_CurrentWorld->CreateEntity();
			return gameObject;
		}

		GameObject Instantiate()
		{
			return m_CurrentWorld->CreateEntity();
		}

		void Destroy(GameObject& gameObject)
		{
			gameObject.Destroy();
		}

		template <typename SystemType>
		void AddSystem()
		{
			m_CurrentWorld->AddSystem(std::make_shared<SystemType>());
		}

		void UpdateWorld(const sf::Time& deltaTime)
		{
			m_CurrentWorld->Update(deltaTime);
		}

		void RenderWorld(sf::RenderTarget& target)
		{
			m_CurrentWorld->Render(target);
		}

		virtual void UpdateEvents(const sf::Event& event) {};
		virtual void Render(sf::RenderTarget& target) {};
	};
} // end of Basic