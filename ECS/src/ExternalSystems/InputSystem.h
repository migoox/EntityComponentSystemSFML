#pragma once
#include "../BasicModules/ECS.h"
#include "../BasicModules/Game.h"
#include "../BasicModules/ResourceManager/ResourceManager.h"
#include "../BasicModules/Components.h"
#include "../BasicModules/Random.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;
using Basic::Game;
using Basic::ResourceManager;
using Basic::Random;

// include external components
#include "../ExternalComponents/Joystick.h"
#include "../ExternalComponents/Stats.h"

class InputSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<Joystick>();
	}

	void Update(const sf::Time& deltaTime) override
	{
		for (auto& gameObject : m_GameObjects)
		{
			auto& joystick = gameObject.GetComponent<Joystick>();

			joystick.WalkUpBtnPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
			joystick.WalkDownBtnPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
			joystick.WalkLeftBtnPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
			joystick.WalkRightBtnPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		}
	}
};