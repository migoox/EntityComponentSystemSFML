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

class RotationalFrictionSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);

		AddToSignature<Basic::RigidBody>();
		AddToSignature<Basic::PhysicMaterial>();
		AddToSignature<Basic::Collider>();
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
				auto& coll = gameObject.GetComponent<Basic::Collider>();

					// if angle velociy is really small (less than 1 deegre/s)
				if (std::abs(rigidBody.AngleVelocity) < 0.25f)
				{
					rigidBody.AngleVelocity = 0.0f;
				}
				else
				{
					float force = mat.DynamicFriction * PhysicsEngine::GravityAccelerationValue() * rigidBody.Mass;
					if (rigidBody.AngleVelocity < 0.0f)
					{
						rigidBody.AddTorqueImpulse(
							force * coll->GetLongestDistanceFromCentroid());
					}
					else
					{
						rigidBody.AddTorqueImpulse(
							-force * coll->GetLongestDistanceFromCentroid());
					}
				}
			}
			else
			{
				// side view friction
			}
		}
	}
};