#pragma once
#include "../../ECS.h"
#include "../../Components.h"
#include "../../Game.h"
#include "../../Maths/Maths.h"
#include "../PhysicsEngine.h"

#include <iostream>

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;

class LinearFrictionSystem : public ECSSystem
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
		using Basic::Maths::NormalizeVector;
		using Basic::Maths::Cross;
		using Basic::PhysicsEngine;

		for (auto& gameObject : m_GameObjects)
		{
			if (PhysicsEngine::IsTopViewToggled())
			{
				// top view friction

					// get items
				auto& mat = gameObject.GetComponent<Basic::PhysicMaterial>();
				auto& rigidBody = gameObject.GetComponent<Basic::RigidBody>();
					
					// opposite direction to velocity of body
				sf::Vector2f direction = -NormalizeVector(rigidBody.Velocity);
				
					// if velociy is really small (less than 2 pixels/s)
				if (Basic::Maths::VectorDistance(rigidBody.Velocity) < 2.0f)
				{
					rigidBody.Velocity = sf::Vector2f();
				}
				else
				{
					rigidBody.AddImpulse(
						mat.DynamicFriction * PhysicsEngine::GravityAccelerationValue() * rigidBody.Mass * direction);
				}
			}
			else
			{
				// side view friction
			}
		}
	}
};