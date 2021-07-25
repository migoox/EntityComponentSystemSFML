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

// external systems/components:

class TestSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<CircleShape>();
	}

	void Update(const sf::Time& deltaTime) override
	{
		auto& circle = m_GameObjects[0];

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			circle.GetTransform().setPosition(Game::MouseWorldPosition());
	}
};