#pragma once
#include <iostream>
#include <windows.h>

#include "../ECS.h"
#include "../Components.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;
using Basic::CollisionPoints;

class CollisionSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<Collider>();
	}

	void Update(const sf::Time& deltaTime) override
	{
		bool collision = false;

		// collision detection 
		for (size_t i = 0; i < m_GameObjects.size(); i++)
		{
			for (size_t j = i + 1; j < m_GameObjects.size(); j++)
			{
				auto& gameObject1 = m_GameObjects.at(i);
				auto& gameObject2 = m_GameObjects.at(j);

				if (gameObject1.IsActive() && gameObject2.IsActive())
				{
					auto& collider1 = gameObject1.GetComponent<Collider>();
					auto& collider2 = gameObject2.GetComponent<Collider>();

					auto& transform1 = gameObject1.GetTransform();
					auto& transform2 = gameObject2.GetTransform();

					if (collider1.Item == nullptr || collider2.Item == nullptr) continue;

					if (!collider1.Item->Active || !collider2.Item->Active) continue;

					CollisionPoints collPoints = collider1.Item->TestCollision(
						transform1, collider2.Item, transform2);

					if (collPoints.HasCollision)
						collision = true;
				}
			}
		}

		if (collision)
			std::cout << "COLLISION DETECTED" << deltaTime.asSeconds() << std::endl;

		// collision resolving
	}
};