#pragma once
#include "../Game.h"
#include "../ECS.h"
#include <memory>
#include <iostream>

using Basic::GameObject;
using Basic::SignatureType;
using Basic::Entity;
using Basic::Game;
using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

class RendererSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<sf::CircleShape>();
	}

	void Render(sf::RenderTarget& target) override
	{
		for (auto& it : m_Entities)
		{
			auto& shape = m_ParentWorld->GetComponent<sf::CircleShape>(it);
			target.draw(shape);
		}
	}
};

class InputSystem : public ECSSystem
{
private:
	void AddBall()
	{
		sf::Vector2u windowSize = Game::WindowSize();

		GameObject gameObject = m_ParentWorld->CreateEntity();

		auto& circleShape = gameObject.AddComponent<sf::CircleShape>(sf::CircleShape());

		circleShape.setFillColor(sf::Color(float(rand() % 255), float(rand() % 255), float(rand() % 255)));
		circleShape.setRadius(15.0f);
		circleShape.setOrigin(15.0f, 15.0f);
		circleShape.setPosition(float(rand() % windowSize.x), float(rand() % windowSize.y));
		std::cout << "add\n";
	}

	void DestroyBall()
	{
		if (m_Entities.size() > 0)
		{
			Entity entity = *m_Entities.begin();
			GameObject gO = m_ParentWorld->GetGameObject(entity);
			gO.Destroy();
			std::cout << "delete\n";
		}
	}

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<sf::CircleShape>();

		// add balls
		for (size_t i = 0; i < 30; i++)
		{
			AddBall();
		}
	}

	void Update(const sf::Time& deltaTime) override
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			DestroyBall();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			AddBall();
		}
	}
};

class MotionSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<sf::CircleShape>();
	}

	void Update(const sf::Time& deltaTime) override
	{
		for (auto& element : m_Entities)
		{
			auto& circleShape = m_ParentWorld->GetComponent<sf::CircleShape>(element);

			sf::Vector2f currentStep = sf::Vector2f(deltaTime.asSeconds() * float(rand() % 100 - 50) / 100.0f, deltaTime.asSeconds() * float(rand() % 100 - 50) / 100.0f);

			currentStep = 20.f * currentStep;
			circleShape.move(currentStep);
		}
	}
};


class GameScene : public Basic::Scene
{
private:

public:
	using Basic::Scene::Scene;
	~GameScene();

	void OnEnter() override;
	void Update() override;
	void OnExit() override;

	void UpdateEvents(const sf::Event& event) override;
	void Render(sf::RenderTarget& target) override;

public:

};