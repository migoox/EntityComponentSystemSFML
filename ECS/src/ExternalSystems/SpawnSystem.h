#pragma once
#include "../BasicModules/ECS.h"
#include "../BasicModules/Game.h"
#include "../BasicModules/Tools/ResourceManager.h"
#include "../BasicModules/Components.h"
#include "../BasicModules/EventSystem/Event.h"

#include <array>

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
	int m_TrianglesCount = 3;
	int m_RectanglesCount = 3;
	int m_SquaresCount = 2;
	int m_CirclesCount = 4;

private:

	std::array<sf::Vector2f, 3> GenerateRandomTrianglePoints(float radius)
	{
		std::array<sf::Vector2f, 3> points;

		float x = std::rand() % (2 * int(radius)) - radius;
		float y = -std::sqrt(std::abs(std::pow(radius, 2) - std::pow(x, 2)));
		points[0] = sf::Vector2f(x, y);

		if (x > 0.0f)
		{
			x = std::rand() % (2 * int(radius)) - radius;
			y = std::sqrt(std::abs(std::pow(radius, 2) - std::pow(x, 2)));

			points[1] = sf::Vector2f(x, y);

			x = std::rand() % (int(radius)) - radius;
			y = -std::sqrt(std::abs(std::pow(radius, 2) - std::pow(x, 2)));

			points[2] = sf::Vector2f(x, y);
		}
		else
		{
			x = std::rand() % (int(radius));
			int randValue = std::rand() % 2;
			if (randValue == 0)
			{
				y = std::sqrt(std::abs(std::pow(radius, 2) - std::pow(x, 2)));
			}
			else
			{
				y = -std::sqrt(std::abs(std::pow(radius, 2) - std::pow(x, 2)));
			}

			points[1] = sf::Vector2f(x, y);

			x = std::rand() % (int(radius)) - radius;
			y = std::sqrt(std::abs(std::pow(radius, 2) - std::pow(x, 2)));

			points[2] = sf::Vector2f(x, y);
		}

		for (auto& point : points)
			point += sf::Vector2f(radius, radius);

		return points;
	}

private:

	void Spawn4VPolygon(sf::Vector2f pos)
	{
		using Basic::Collider;
		using Basic::Shape;
		using Basic::RigidBody;
		using Basic::PhysicMaterial;

		GameObject pol = Instantiate();

		sf::Vector2f size = sf::Vector2f(float(rand() % 70) + 20.0f,
			float(rand() % 60) + 50.0f);

		auto& gI = pol.AddComponent<GrabbableElement>(GrabbableElement());
		gI.DeafultColor = sf::Color(0.0f, float(rand() % 155) + 50.0f, float(rand() % 155) + 50.0f, 200.0f);

		auto& coll = pol.AddComponent<Collider>(new Basic::PolygonCollider(
			{
				sf::Vector2f(0.0f, 0.0f),
				sf::Vector2f(300.0f, 100.0f),
				sf::Vector2f(200.0f, 200.0f),
				sf::Vector2f(10.0f, 50.0f)
			}));

		auto& shape = pol.AddComponent<Shape>(new Basic::PolygonShape({
				sf::Vector2f(0.0f, 0.0f),
				sf::Vector2f(300.0f, 100.0f),
				sf::Vector2f(200.0f, 200.0f),
				sf::Vector2f(10.0f, 50.0f)
			}));

		shape->SetFillColor(gI.DeafultColor);

		auto& mat = pol.AddComponent<PhysicMaterial>();
		mat.Bounciness = 0.7f;
		mat.DynamicFriction = 0.15f;

		pol.AddComponent<RigidBody>(RigidBody());

		pol.GetTransform().setScale(0.5f, 0.5f);
		pol.GetTransform().setPosition(pos);
	}

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
		using Basic::PhysicMaterial;

		GameObject triangle = Instantiate();

		auto vertices = GenerateRandomTrianglePoints(float(rand() % 20) + 100.0f);

		sf::Vector2f size = sf::Vector2f(float(rand() % 70) + 20.0f,
			float(rand() % 60) + 50.0f);


		auto& gI = triangle.AddComponent<GrabbableElement>(GrabbableElement());
		gI.DeafultColor = sf::Color(0.0f, float(rand() % 155) + 50.0f, float(rand() % 155) + 50.0f, 200.0f);

		auto& coll = triangle.AddComponent<Collider>(new Basic::PolygonCollider(
			{vertices[0], vertices[1], vertices[2] }));

		auto& shape = triangle.AddComponent<Shape>(new Basic::PolygonShape(
			{ vertices[0], vertices[1], vertices[2] }));

		shape->SetFillColor(gI.DeafultColor);

		triangle.AddComponent<RigidBody>();

		auto& mat = triangle.AddComponent<PhysicMaterial>();
		mat.Bounciness = 0.7f;
		mat.DynamicFriction = 0.15f;

		triangle.GetTransform().setPosition(pos);

		triangle.GetTransform().setScale(0.5f, 0.5f);
	}

	void SpawnRandRectangle(sf::Vector2f pos)
	{
		using Basic::Collider;
		using Basic::Shape;
		using Basic::RigidBody;
		using Basic::PhysicMaterial;

		GameObject rect = Instantiate();

		sf::Vector2f size = sf::Vector2f(float(rand() % 70) + 20.0f,
			float(rand() % 60) + 50.0f);

		auto& gI = rect.AddComponent<GrabbableElement>(GrabbableElement());
		gI.DeafultColor = sf::Color(0.0f, float(rand() % 155) + 50.0f, float(rand() % 155) + 50.0f, 200.0f);

		auto& coll = rect.AddComponent<Collider>(new Basic::RectangleCollider(size));

		auto& shape = rect.AddComponent<Shape>(new Basic::BoxShape(size));
		shape->SetFillColor(gI.DeafultColor);

		auto& mat = rect.AddComponent<PhysicMaterial>();
		mat.Bounciness = 0.7f;
		mat.DynamicFriction = 0.15f;

		rect.AddComponent<RigidBody>(RigidBody());

		rect.GetTransform().setPosition(pos);
	}

	void SpawnRandSquare(sf::Vector2f pos)
	{
		using Basic::Collider;
		using Basic::Shape;
		using Basic::RigidBody;
		using Basic::PhysicMaterial;

		GameObject square = Instantiate();

		float a = float(rand() % 40) + 20.0f;
		sf::Vector2f size = sf::Vector2f(a, a);

		auto& gI = square.AddComponent<GrabbableElement>(GrabbableElement());
		gI.DeafultColor = sf::Color(0.0f, float(rand() % 155) + 50.0f, float(rand() % 155) + 50.0f, 200.0f);

		auto& coll = square.AddComponent<Collider>(new Basic::RectangleCollider(size));

		auto& shape = square.AddComponent<Shape>(new Basic::BoxShape(size));
		shape->SetFillColor(gI.DeafultColor);

		square.AddComponent<RigidBody>();

		auto& mat = square.AddComponent<PhysicMaterial>();
		mat.Bounciness = 0.7f;
		mat.DynamicFriction = 0.15f;

		square.GetTransform().setPosition(pos);
	}

	void SpawnRandCircle(sf::Vector2f pos)
	{
		using Basic::Collider;
		using Basic::Shape;
		using Basic::RigidBody;
		using Basic::PhysicMaterial;

		GameObject circle = Instantiate();

		float radius = float(rand() % 20) + 20.0f;

		auto& gI = circle.AddComponent<GrabbableElement>(GrabbableElement());
		gI.DeafultColor = sf::Color(0.0f, float(rand() % 155) + 50.0f, float(rand() % 155) + 50.0f, 200.0f);

		auto& coll = circle.AddComponent<Collider>(new Basic::CircleCollider(radius));

		auto& shape = circle.AddComponent<Shape>(new Basic::CircleShape(radius));
		shape->SetFillColor(gI.DeafultColor);

		circle.AddComponent<RigidBody>();

		auto& mat = circle.AddComponent<PhysicMaterial>();
		mat.Bounciness = 0.7f;
		mat.DynamicFriction = 0.15f;

		circle.GetTransform().setPosition(pos);
	}

	void Spawn()
	{
		sf::Vector2u windowSize = Game::WindowSize();

		SpawnStaticWalls();
		Spawn4VPolygon(sf::Vector2f(rand() % windowSize.x, rand() % windowSize.y));
		// spawn entites
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

public:
	void Init() override
	{
		SetSignatureType(SignatureType::None);
	}

	void OnStart() override
	{
		Spawn();
	}
};