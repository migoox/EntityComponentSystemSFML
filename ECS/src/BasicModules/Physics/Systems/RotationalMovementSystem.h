#pragma once
#include "../../ECS.h"
#include "../../Components.h"
#include "../../Game.h"
#include "../PhysicsEngine.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;

class RotationalMovementSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);

		AddToSignature<Basic::RigidBody>();
		AddToSignature<Basic::Collider>();
	}

	void Update(const sf::Time& deltaTime) override
	{
		using Basic::RigidBody;
		using Basic::Collider;
		using Basic::PhysicsEngine;

		for (auto& gameObject : m_GameObjects)
		{
			auto& rigidBody = gameObject.GetComponent<RigidBody>();
			auto& collider = gameObject.GetComponent<Collider>();

			float momentOfIntertia = collider->GetMomentOfInertia(rigidBody);
			
			// update rotational acceleration (epsilon)
			if (!rigidBody.FreezeRotation)
				rigidBody.AngleAcceleration = rigidBody.Torque / momentOfIntertia;

			// update velocity
				// acceleration
			if (!rigidBody.FreezeXAxisMovement)
				rigidBody.AngleVelocity += rigidBody.AngleAcceleration * deltaTime.asSeconds();

			if (!rigidBody.FreezeYAxisMovement)
				rigidBody.AngleVelocity += rigidBody.AngleAcceleration * deltaTime.asSeconds();
				// impulse
			if (!rigidBody.FreezeXAxisMovement)
				rigidBody.AngleVelocity += rigidBody.TorqueImpulse * deltaTime.asSeconds() / momentOfIntertia;

			if (!rigidBody.FreezeYAxisMovement)
				rigidBody.AngleVelocity += rigidBody.TorqueImpulse * deltaTime.asSeconds() / momentOfIntertia;

			// delete impulse after frame
			rigidBody.TorqueImpulse = 0.0f;

			// rotate rigidBody
			gameObject.GetTransform().rotate(rigidBody.AngleVelocity * 180.0f / 3.14159265f * deltaTime.asSeconds());
		}
	}
};