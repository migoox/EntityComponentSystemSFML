#pragma once
#include "../ECS.h"
#include "../Components.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;

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

			// update acceleration
			rigidBody.Acceleration = rigidBody.Force / rigidBody.Mass;

			// create resultant acceleration (gravity + acceleration stored in rigidbody)
			sf::Vector2f resultantAcceleration = rigidBody.Acceleration;

			if (rigidBody.UseGravity)
				resultantAcceleration += GravityAcceleration;

			// update velocity
			rigidBody.Velocity += resultantAcceleration * deltaTime.asSeconds();

			// move rigidBody
			gameObject.GetTransform().move(rigidBody.Velocity * deltaTime.asSeconds());
		}
	}
};