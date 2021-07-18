#pragma once
#include "../BasicModules/ECS.h"
#include "../BasicModules/Game.h"
#include "../BasicModules/ResourceManager/ResourceManager.h"
#include "../BasicModules/Components.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::SignatureType;
using Basic::Entity;
using Basic::Game;
using Basic::ResourceManager;

class InputSystem : public ECSSystem
{
private:
	std::shared_ptr<sf::Texture> m_Texture;

private:
	void AddBall()
	{
		sf::Vector2u windowSize = Game::WindowSize();

		GameObject gameObject = m_ParentWorld->CreateEntity();

		// transform is added by default
		auto& transform = gameObject.GetComponent<Transform>();

		auto& circleShape = gameObject.AddComponent<Sprite>(Sprite());

		circleShape.setTexture(*m_Texture);

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
			std::cout << "deleted: " << entity << "\n";
		}
	}

	bool deleteClicked = false;
	bool addClicked = false;

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<Transform>();

		m_Texture = ResourceManager::TextureAcquire("resources/enemy.png");
	}

	void Update(const sf::Time& deltaTime) override
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			if (!deleteClicked)
				DestroyBall();
			deleteClicked = true;
		}
		else
		{
			deleteClicked = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			if (!addClicked)
				AddBall();
			addClicked = true;
		}
		else
		{
			addClicked = false;
		}
	}
};