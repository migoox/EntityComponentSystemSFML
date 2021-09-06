#pragma once
#include "../../ECS.h"
#include "../../Components.h"
#include "../../Game.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;


class TopViewFrictionSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);

		AddToSignature<Basic::RigidBody>();
		AddToSignature<Basic::PhysicMaterial>();
	}

	void Update(const sf::Time& deltaTime) override
	{
				
	}
};