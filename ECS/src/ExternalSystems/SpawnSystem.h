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
#include "../ExternalComponents/GrabbableElement.h"

using namespace Basic::Components;

class SpawnSystem : public ECSSystem
{
private:
	int m_4VPolygonsCount = 2;
	int m_TrianglesCount = 5;
	int m_RectanglesCount = 3;
	int m_SquaresCount = 2;
	int m_CirclesCount = 4;

private:
	void SpawnRand4VPolygon(sf::Vector2f pos)
	{

	}

	void SpawnRandTriangle(sf::Vector2f pos)
	{

	}

	void SpawnRandRectangle(sf::Vector2f pos)
	{
		GameObject rect = Instantiate();

		sf::Vector2f size = sf::Vector2f(float(rand() % 130) + 20.0f,
			float(rand() % 130) + 50.0f);

		rect.AddComponent<GrabbableElement>(GrabbableElement());
		auto& coll = rect.AddComponent<Collider>(new RectangleCollider(size));
		auto& shape = rect.AddComponent<RectangleShape>(RectangleShape());
		// rect.AddComponent<RigidBody>(RigidBody());

		shape.setPosition(pos);
		shape.setFillColor(sf::Color(0.0f, float(rand() % 155) + 50.0f, float(rand() % 155) + 50.0f, 200.0f));
		shape.setSize(size);
		shape.setOrigin(size.x / 2.0f, size.y / 2.0f);
	}

	void SpawnRandSquare(sf::Vector2f pos)
	{
		GameObject square = Instantiate();

		float a = float(rand() % 130) + 20.0f;
		sf::Vector2f size = sf::Vector2f(a, a);

		square.AddComponent<GrabbableElement>(GrabbableElement());
		auto& coll = square.AddComponent<Collider>(new RectangleCollider(size));
		auto& shape = square.AddComponent<RectangleShape>(RectangleShape());
		square.AddComponent<RigidBody>(RigidBody());

		shape.setPosition(pos);
		shape.setFillColor(sf::Color(0.0f, float(rand() % 155) + 50.0f, float(rand() % 155) + 50.0f, 200.0f));
		shape.setSize(size);
		shape.setOrigin(size.x / 2.0f, size.y / 2.0f);
	}

	void SpawnRandCircle(sf::Vector2f pos)
	{
		GameObject circle = Instantiate();

		float a = float(rand() % 130) + 20.0f;
		sf::Vector2f size = sf::Vector2f(a, a);

		circle.AddComponent<GrabbableElement>(GrabbableElement());
		auto& coll = circle.AddComponent<Collider>(new RectangleCollider(size));
		auto& shape = circle.AddComponent<RectangleShape>(RectangleShape());
		circle.AddComponent<RigidBody>(RigidBody());

		shape.setPosition(pos);
		shape.setFillColor(sf::Color(0.0f, float(rand() % 155) + 50.0f, float(rand() % 155) + 50.0f, 200.0f));
		shape.setSize(size);
		shape.setOrigin(size.x / 2.0f, size.y / 2.0f);
	}

public:
	void Init() override
	{
		// no entites assigned
		SetSignatureType(SignatureType::None);
	}

	void OnStart() override
	{
		sf::Vector2u windowSize = Game::WindowSize();

		// spawn entites
		for (size_t i = 0; i < m_4VPolygonsCount; i++)
		{

		}

		for (size_t i = 0; i < m_TrianglesCount; i++)
		{

		}

		for (size_t i = 0; i < m_RectanglesCount; i++)
		{
			SpawnRandRectangle(sf::Vector2f(rand() % windowSize.x, rand() % windowSize.y));
		}

		for (size_t i = 0; i < m_SquaresCount; i++)
		{
			//SpawnRandSquare(sf::Vector2f(rand() % windowSize.x, rand() % windowSize.y));
		}

		for (size_t i = 0; i < m_CirclesCount; i++)
		{
			//SpawnRandCircle(sf::Vector2f(rand() % windowSize.x, rand() % windowSize.y));
		}

	}

};