#pragma once
#include "../BasicModules/ECS.h"
#include "../BasicModules/Game.h"
#include "../BasicModules/ResourceManager/ResourceManager.h"
#include "../BasicModules/Components.h"
#include "../BasicModules/EventSystem/Event.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;
using Basic::Game;
using Basic::Event;
using Basic::ResourceManager;

class TestEvent : public Event
{
public:
	int intNum;
	TestEvent(int num) : intNum(num) {};
};

class MotionSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<Transform>();

		m_EventBus->AddListener<TestEvent>(
			[](TestEvent* ev)
			{
				std::cout << "listener1:\n";
				std::cout << ev->intNum << std::endl;
			});
	}

	void Update(const sf::Time& deltaTime) override
	{
		for (auto& gameObject : m_GameObjects)
		{
			if (gameObject.IsActive())
			{
				auto& transform = gameObject.GetTransform();

				sf::Vector2f currentStep = sf::Vector2f(deltaTime.asSeconds() * float(rand() % 100 - 50) / 100.0f, deltaTime.asSeconds() * float(rand() % 100 - 50) / 100.0f);

				currentStep = 350.f * currentStep;
				transform.move(currentStep);
			}
		}
	}
};