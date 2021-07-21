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

class PlayerSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Constitutive); 

		AddToSignature<Transform>();
		AddToSignature<Animator>();
		AddToSignature<Sprite>();
		AddToSignature<RigidBody>();
		AddToSignature<Stats>();
		AddToSignature<Joystick>();
	}

	void Update(const sf::Time& deltaTime) override
	{

		for (auto& gameObject : m_GameObjects)
		{
			// get Components
			auto& transform = gameObject.GetTransform();
			auto& animator = gameObject.GetComponent<Animator>();
			auto& rigidBody = gameObject.GetComponent<RigidBody>();
			auto& joystick = gameObject.GetComponent<Joystick>();
			auto& stats = gameObject.GetComponent<Stats>();

			// give player velocity
			rigidBody.Velocity = sf::Vector2f(0.f, 0.f);

			if (joystick.WalkRightBtnPressed)
				rigidBody.Velocity.x += stats.WalkVelocity;
			if (joystick.WalkLeftBtnPressed)
				rigidBody.Velocity.x += -stats.WalkVelocity;
			if (joystick.WalkUpBtnPressed)
				rigidBody.Velocity.y += -stats.WalkVelocity;
			if (joystick.WalkDownBtnPressed)
				rigidBody.Velocity.y += stats.WalkVelocity;

			// set player horizontal direction
			if (rigidBody.Velocity.x > 0.f)
				transform.setScale(-std::abs(transform.getScale().x), transform.getScale().y);
			else
				transform.setScale(std::abs(transform.getScale().x), transform.getScale().y);

			// animations
			if (rigidBody.Velocity.x == 0.f && rigidBody.Velocity.y == 0.f)
			{
				// if player doesn't move play idle
				animator.Play("IDLE");
			}
			else
			{
				// if player moves choose correct animation
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