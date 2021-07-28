#pragma once
#include "../BasicModules/ECS.h"
#include "../BasicModules/Game.h"
#include "../BasicModules/Tools/ResourceManager.h"
#include "../BasicModules/Components.h"
#include "../BasicModules/EventSystem/Event.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;
using Basic::Game;
using Basic::Event;
using Basic::ResourceManager;
using Basic::ColliderItem;

// external systems/components:
#include "../ExternalComponents/Ball.h"

using namespace Basic::Components;

class BallsSystem : public ECSSystem
{
private:
	GameObject m_StretchingGum;

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<Collider>();
		AddToSignature<CircleShape>();
		AddToSignature<RigidBody>();
		AddToSignature<Ball>();

	}

	void OnStart() override
	{
		Instantiate(m_StretchingGum);
		m_StretchingGum.AddComponent<Line>(Line(100.0f, 20.0f, sf::Color::Green));
		m_StretchingGum.SetVisibility(false);
	}

	void Update(const sf::Time& deltaTime) override
	{
		for (auto& gameObject : m_GameObjects)
		{
			auto& ball = gameObject.GetComponent<Ball>();
			auto& circleShape = gameObject.GetComponent<CircleShape>();

			if (ball.Selected)
			{
				circleShape.setFillColor(sf::Color(20, 100, 245, 245));
				if (ball.FitMousePosition)
				{
					gameObject.GetTransform().setPosition(Game::MouseWorldPosition());
					gameObject.GetComponent<RigidBody>().Velocity = sf::Vector2f();
				}
				else
				{
					auto& line = m_StretchingGum.GetComponent<Line>();

					if (ball.SlingshotStretchingHappens) // stretch
					{
						if (ball.SlingshotStretchingStarted) // start
						{
							m_StretchingGum.SetVisibility(true);
						}

						line.setPoints(gameObject.GetTransform().getPosition(), Game::MouseWorldPosition());
					}
					else if(ball.SlingshotStretchingFinished) // shoot
					{
						m_StretchingGum.SetVisibility(false);

						gameObject.GetComponent<RigidBody>().Velocity = 
							gameObject.GetTransform().getPosition() - Game::MouseWorldPosition();
					}
				}
			}
			else
			{
				circleShape.setFillColor(sf::Color(0, 0, 0, 245));
			}
		}
	}

};