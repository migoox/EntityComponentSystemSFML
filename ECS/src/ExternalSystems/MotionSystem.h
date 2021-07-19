#pragma once
#include "../BasicModules/ECS.h"
#include "../BasicModules/Game.h"
#include "../BasicModules/ResourceManager/ResourceManager.h"
#include "../BasicModules/Components.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;
using Basic::Game;
using Basic::ResourceManager;

class MotionSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<Transform>();
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