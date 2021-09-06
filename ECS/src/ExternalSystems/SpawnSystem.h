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

class SpawnSystem : public ECSSystem
{
private:
	int m_4VPolygonsCount = 2;
	int m_TrianglesCount = 3;
	int m_RectanglesCount = 3;
	int m_SquaresCount = 2;
	int m_CirclesCount = 4;

private:

	void SpawnStaticWalls()
	{
		using Basic::Collider;
		using Basic::Shape;
		using Basic::RigidBody;

		sf::Vector2u windowSize = Game::WindowSize();

		// left wall:
		GameObject wall1 = Instantiate();

		auto& shape1 = wall1.AddComponent<Shape>(new Basic::LineShape(windowSize.y, 4.0f));
		shape1->SetFillColor(sf::Color::Black);

		auto& coll1 = wall1.AddComponent<Collider>(new Basic::PlaneCollider(windowSize.y));
		coll1->Movable = false;
		coll1->Rotatable = false;

		wall1.AddComponent<RigidBody>();
		
		wall1.GetTransform().setPosition(sf::Vector2f(0.0f, windowSize.y / 2.0f));
		wall1.GetTransform().setRotation(90.0f);

		// right wall:
		GameObject wall2 = Instantiate();

		auto& shape2 = wall2.AddComponent<Shape>(new Basic::LineShape(windowSize.y, 4.0f));
		shape2->SetFillColor(sf::Color::Black);

		auto& coll2 = wall2.AddComponent<Collider>(new Basic::PlaneCollider(windowSize.y));
		coll2->Movable = false;
		coll2->Rotatable = false;

		wall2.AddComponent<RigidBody>();

		wall2.GetTransform().setPosition(sf::Vector2f(windowSize.x, windowSize.y / 2.0f));
		wall2.GetTransform().setRotation(90.0f);

		// top wall:
		GameObject wall3 = Instantiate();

		auto& shape3 = wall3.AddComponent<Shape>(new Basic::LineShape(windowSize.x, 4.0f));
		shape3->SetFillColor(sf::Color::Black);

		auto& coll3 = wall3.AddComponent<Collider>(new Basic::PlaneCollider(windowSize.x));
		coll3->Movable = false;
		coll3->Rotatable = false;

		wall3.AddComponent<RigidBody>();

		wall3.GetTransform().setPosition(sf::Vector2f(windowSize.x / 2.0f, 0.0f));

		// bottom wall:
		GameObject wall4 = Instantiate();

		auto& shape4 = wall4.AddComponent<Shape>(new Basic::LineShape(windowSize.x, 4.0f));
		shape4->SetFillColor(sf::Color::Black);

		auto& coll4 = wall4.AddComponent<Collider>(new Basic::PlaneCollider(windowSize.x));
		coll4->Movable = false;
		coll4->Rotatable = false;

		wall4.AddComponent<RigidBody>();

		wall4.GetTransform().setPosition(sf::Vector2f(windowSize.x / 2.0f, windowSize.y));
	}

	void SpawnRandTriangle(sf::Vector2f pos)
	{
		using Basic::Collider;
		using Basic::Shape;
		using Basic::RigidBody;

		GameObject rect = Instantiate();

		sf::Vector2f size = sf::Vector2f(float(rand() % 70) + 20.0f,
			float(rand() % 60) + 50.0f);

		auto& gI = rect.AddComponent<GrabbableElement>(GrabbableElement());
		gI.DeafultColor = sf::Color(0.0f, float(rand() % 155) + 50.0f, float(rand() % 155) + 50.0f, 200.0f);

		auto& coll = rect.AddComponent<Collider>(new Basic::PolygonCollider(
			{sf::Vector2f(50.0f, 0.0f), sf::Vector2f(150.0f, 100.0f), sf::Vector2f(0.0f, 150.0f) }));

		auto& shape = rect.AddComponent<Shape>(new Basic::PolygonShape(
			{ sf::Vector2f(50.0f, 0.0f), sf::Vector2f(150.0f, 100.0f), sf::Vector2f(0.0f, 150.0f) }));

		shape->SetFillColor(gI.DeafultColor);

		rect.AddComponent<RigidBody>(RigidBody());

		rect.GetTransform().setPosition(pos);

		rect.GetTransform().setScale(0.5f, 0.5f);
	}

	void SpawnRandRectangle(sf::Vector2f pos)
	{
		using Basic::Collider;
		using Basic::Shape;
		using Basic::RigidBody;

		GameObject rect = Instantiate();

		sf::Vector2f size = sf::Vector2f(float(rand() % 70) + 20.0f,
			float(rand() % 60) + 50.0f);

		auto& gI = rect.AddComponent<GrabbableElement>(GrabbableElement());
		gI.DeafultColor = sf::Color(0.0f, float(rand() % 155) + 50.0f, float(rand() % 155) + 50.0f, 200.0f);

		auto& coll = rect.AddComponent<Collider>(new Basic::RectangleCollider(size));

		auto& shape = rect.AddComponent<Shape>(new Basic::BoxShape(size));
		shape->SetFillColor(gI.DeafultColor);

		rect.AddComponent<RigidBody>(RigidBody());

		rect.GetTransform().setPosition(pos);
	}

	void SpawnRandSquare(sf::Vector2f pos)
	{
		using Basic::Collider;
		using Basic::Shape;
		using Basic::RigidBody;

		GameObject square = Instantiate();

		float a = float(rand() % 40) + 20.0f;
		sf::Vector2f size = sf::Vector2f(a, a);

		auto& gI = square.AddComponent<GrabbableElement>(GrabbableElement());
		gI.DeafultColor = sf::Color(0.0f, float(rand() % 155) + 50.0f, float(rand() % 155) + 50.0f, 200.0f);

		auto& coll = square.AddComponent<Collider>(new Basic::RectangleCollider(size));

		auto& shape = square.AddComponent<Shape>(new Basic::BoxShape(size));
		shape->SetFillColor(gI.DeafultColor);

		square.AddComponent<RigidBody>(RigidBody());


		square.GetTransform().setPosition(pos);
	}

	void SpawnRandCircle(sf::Vector2f pos)
	{
		using Basic::Collider;
		using Basic::Shape;
		using Basic::RigidBody;

		GameObject circle = Instantiate();

		float radius = float(rand() % 20) + 20.0f;

		auto& gI = circle.AddComponent<GrabbableElement>(GrabbableElement());
		gI.DeafultColor = sf::Color(0.0f, float(rand() % 155) + 50.0f, float(rand() % 155) + 50.0f, 200.0f);

		auto& coll = circle.AddComponent<Collider>(new Basic::CircleCollider(radius));

		auto& shape = circle.AddComponent<Shape>(new Basic::CircleShape(radius));
		shape->SetFillColor(gI.DeafultColor);

		circle.AddComponent<RigidBody>(RigidBody());

		circle.GetTransform().setPosition(pos);
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

		SpawnStaticWalls();

		// spawn entites
		for (size_t i = 0; i < m_4VPolygonsCount; i++)
		{

		}

		for (size_t i = 0; i < m_TrianglesCount; i++)
		{
			SpawnRandTriangle(sf::Vector2f(rand() % windowSize.x, rand() % windowSize.y));
		}

		for (size_t i = 0; i < m_RectanglesCount; i++)
		{
			SpawnRandRectangle(sf::Vector2f(rand() % windowSize.x, rand() % windowSize.y));
		}

		for (size_t i = 0; i < m_SquaresCount; i++)
		{
			SpawnRandSquare(sf::Vector2f(rand() % windowSize.x, rand() % windowSize.y));
		}

		for (size_t i = 0; i < m_CirclesCount; i++)
		{
			SpawnRandCircle(sf::Vector2f(rand() % windowSize.x, rand() % windowSize.y));
		}

	}

};