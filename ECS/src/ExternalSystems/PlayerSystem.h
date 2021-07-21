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

class PlayerSystem : public ECSSystem
{
private:
	bool m_WalkUp;
	bool m_WalkDown;
	bool m_WalkLeft;
	bool m_WalkRight;

	bool m_Kick;

	float m_PlayerVelocity = 200.0f;

private: 
	void UpdateInput()
	{
		m_WalkUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		m_WalkDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
		m_WalkLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		m_WalkRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

		m_Kick = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	}

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Constitutive);

		AddToSignature<Transform>();
		AddToSignature<Animator>();
		AddToSignature<Sprite>();
		AddToSignature<RigidBody>();
	}

	void Update(const sf::Time& deltaTime) override
	{
		UpdateInput();

		for (auto& gameObject : m_GameObjects)
		{
			auto& transform = gameObject.GetComponent<Transform>();
			auto& animator = gameObject.GetComponent<Animator>();
			auto& rigidBody = gameObject.GetComponent<RigidBody>();

			// game logic 
			rigidBody.Velocity = sf::Vector2f(0.f, 0.f);

			if (m_WalkRight)
				rigidBody.Velocity.x += m_PlayerVelocity;
			if (m_WalkLeft)
				rigidBody.Velocity.x += -m_PlayerVelocity;
			if (m_WalkUp)
				rigidBody.Velocity.y += -m_PlayerVelocity;
			if (m_WalkDown)
				rigidBody.Velocity.y += m_PlayerVelocity;

			// set player horizontal direction
			if (rigidBody.Velocity.x > 0.f)
			{
				transform.setScale(-std::abs(transform.getScale().x), 
					transform.getScale().y);
			}
			else
			{
				transform.setScale(std::abs(transform.getScale().x), 
					transform.getScale().y);
			}

			// move player using velocity
			transform.move(rigidBody.Velocity * deltaTime.asSeconds());

			// animations
			if (m_Kick)
			{
				animator.PlayOnce("KICK");
			}
			else if (rigidBody.Velocity.x == 0.f && rigidBody.Velocity.y == 0.f)
			{
				animator.Play("IDLE");
			}
			else
			{
				if (rigidBody.Velocity.x != 0.f)
					animator.Play("HORIZONTAL_WALK");
				else if (rigidBody.Velocity.y > 0.f)
					animator.Play("FRONT_WALK");
				else if (rigidBody.Velocity.y < 0.f)
					animator.Play("BACK_WALK");
			}
		}
	}
};