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

class MotionSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);

		AddToSignature<Transform>();
		AddToSignature<RigidBody>();
	}

	void Update(const sf::Time& deltaTime) override
	{
		for (auto& gameObject : m_GameObjects)
		{
			auto& transform = gameObject.GetTransform();
			auto& rigidBody = gameObject.GetComponent<RigidBody>();

			transform.move(rigidBody.Velocity * deltaTime.asSeconds());
		}
	}
};