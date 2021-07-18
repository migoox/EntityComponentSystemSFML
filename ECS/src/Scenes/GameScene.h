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
		AddToSignature<sf::Transformable>();
		AddToSignature<sf::CircleShape>();
	}

	void Render(sf::RenderTarget& target) override
	{
		for (auto& it : m_Entities)
		{
			auto& shape = m_ParentWorld->GetComponent<sf::CircleShape>(it);
			auto& transform = m_ParentWorld->GetComponent<sf::Transformable>(it);
			
			target.draw(shape, sf::RenderStates(transform.getTransform()));
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

		auto& transform = gameObject.AddComponent<sf::Transformable>(sf::Transformable());
		auto& circleShape = gameObject.AddComponent<sf::CircleShape>(sf::CircleShape());

		circleShape.setFillColor(sf::Color(float(rand() % 255), float(rand() % 255), float(rand() % 255)));
		circleShape.setRadius(15.0f);
		circleShape.setOrigin(15.0f, 15.0f);

		transform.setPosition(float(rand() % windowSize.x) / 2 + windowSize.x / 4, 
			float(rand() % windowSize.y) / 2 + windowSize.y / 4);

		std::cout << "add: " << gameObject.ThisEntity << "\n";
	}

	void DestroyBall()
	{
		if (m_Entities.size() > 0)
		{
			Entity entity = *m_Entities.begin();
			GameObject gO = m_ParentWorld->GetGameObject(entity);
			gO.Destroy();
			std::cout << "deleted: "<<entity<<"\n";
		}
	}

	bool deleteClicked = false;
	bool addClicked = false;

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<sf::Transformable>();

		// add balls
		/*for (size_t i = 0; i < 30; i++)
		{
			AddBall();
		}*/
	}

	void Update(const sf::Time& deltaTime) override
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			if(!deleteClicked)
				DestroyBall();
			deleteClicked = true;
		}
		else
		{
			deleteClicked = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			if(!addClicked)
				AddBall();
			addClicked = true;
		}
		else
		{
			addClicked = false;
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
		AddToSignature<sf::Transformable>();
	}

	void Update(const sf::Time& deltaTime) override
	{
		for (auto& element : m_Entities)
		{
			auto& transform = m_ParentWorld->GetComponent<sf::Transformable>(element);

			sf::Vector2f currentStep = sf::Vector2f(deltaTime.asSeconds() * float(rand() % 100 - 50) / 100.0f, deltaTime.asSeconds() * float(rand() % 100 - 50) / 100.0f);

			currentStep = 20.f * currentStep;
			transform.move(currentStep);
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