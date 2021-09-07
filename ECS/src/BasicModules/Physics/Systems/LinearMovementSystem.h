#pragma once
#include "../../ECS.h"
#include "../../Components.h"
#include "../../Game.h"
#include "../PhysicsEngine.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;


class LinearMovementSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);

		AddToSignature<Basic::RigidBody>();
	}

	void Update(const sf::Time& deltaTime) override
	{
		using Basic::RigidBody;
		using Basic::PhysicsEngine;

		for (auto& gameObject : m_GameObjects)
		{
			auto& rigidBody = gameObject.GetComponent<RigidBody>();
			
			/* LINEAR MOVEMENT */

			// update acceleration
			if(!rigidBody.FreezeXAxisMovement)
				rigidBody.Acceleration.x = rigidBody.Force.x / rigidBody.Mass;

			if (!rigidBody.FreezeYAxisMovement)
				rigidBody.Acceleration.y = rigidBody.Force.y / rigidBody.Mass;

			// create resultant acceleration (gravity + acceleration stored in rigidbody)
			sf::Vector2f resultantAcceleration = rigidBody.Acceleration;

			if (rigidBody.UseGravity)
			{
				if (!rigidBody.FreezeXAxisMovement)
					resultantAcceleration.x += PhysicsEngine::GravityAccelerationValue() * 
					PhysicsEngine::GravityAccelerationDirection().x;

				if (!rigidBody.FreezeYAxisMovement)
					resultantAcceleration.y += PhysicsEngine::GravityAccelerationValue() * 
					PhysicsEngine::GravityAccelerationDirection().y;
			}

			// update velocity
				// acceleration
			if (!rigidBody.FreezeXAxisMovement)
				rigidBody.Velocity.x += resultantAcceleration.x * deltaTime.asSeconds();

			if (!rigidBody.FreezeYAxisMovement)
				rigidBody.Velocity.y += resultantAcceleration.y * deltaTime.asSeconds();
				// impulse
			if (!rigidBody.FreezeXAxisMovement)
				rigidBody.Velocity.x += rigidBody.Impulse.x * deltaTime.asSeconds() / rigidBody.Mass;

			if (!rigidBody.FreezeYAxisMovement)
				rigidBody.Velocity.y += rigidBody.Impulse.y * deltaTime.asSeconds() / rigidBody.Mass;

			// delete impulse after frame
			rigidBody.Impulse = sf::Vector2f();

			// move rigidBody
			if (!rigidBody.FreezeXAxisMovement)
				gameObject.GetTransform().move(rigidBody.Velocity.x * deltaTime.asSeconds(), 0.0f);

			if (!rigidBody.FreezeYAxisMovement)
				gameObject.GetTransform().move(0.0f, rigidBody.Velocity.y * deltaTime.asSeconds());
		}
	}
};