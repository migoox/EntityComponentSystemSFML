#pragma once
#include "../../ECS.h"
#include "../../Components.h"
#include "../../Game.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;

using namespace Basic::Components;

class PhysicsSystem : public ECSSystem
{
private:
	sf::Vector2f GravityAcceleration = sf::Vector2f(0.0f, 1000.0f);

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);

		AddToSignature<RigidBody>();
	}

	void Update(const sf::Time& deltaTime) override
	{
		for (auto& gameObject : m_GameObjects)
		{
			auto& rigidBody = gameObject.GetComponent<RigidBody>();
			
			/* MOVEMENT */

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
					resultantAcceleration.x += GravityAcceleration.x;

				if (!rigidBody.FreezeYAxisMovement)
					resultantAcceleration.y += GravityAcceleration.y;
			}

			// update velocity
			if (!rigidBody.FreezeXAxisMovement)
				rigidBody.Velocity.x += resultantAcceleration.x * deltaTime.asSeconds();

			if (!rigidBody.FreezeYAxisMovement)
				rigidBody.Velocity.y += resultantAcceleration.y * deltaTime.asSeconds();

			// move rigidBody
			if (!rigidBody.FreezeXAxisMovement)
				gameObject.GetTransform().move(rigidBody.Velocity.x * deltaTime.asSeconds(), 0.0f);

			if (!rigidBody.FreezeYAxisMovement)
				gameObject.GetTransform().move(0.0f, rigidBody.Velocity.y * deltaTime.asSeconds());

			/* ROTATION */

			// rotate
			gameObject.GetTransform().rotate(rigidBody.AngleVelocity * deltaTime.asSeconds());
		}
	}
};