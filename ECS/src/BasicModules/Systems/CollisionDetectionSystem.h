#pragma once
#include <iostream>
#include <windows.h>

#include "../ECS.h"
#include "../Components.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;

class CollisionDetectionSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);

		AddToSignature<CircleCollider>();
	}

	void Update(const sf::Time& deltaTime) override
	{
		/*auto& object1 = m_GameObjects[0]; // blue
		auto& collider1 = object1.GetComponent<CircleCollider>();
		auto& object2 = m_GameObjects[1]; // red
		auto& collider2 = object2.GetComponent<CircleCollider>();

		auto collPoints = collider1.TestCollision(object1.GetTransform(),
			collider2,
			object2.GetTransform());

		if (collPoints.HasCollision)
			std::cout << "COLLISION DETECTED: " << deltaTime.asSeconds() << "\n";*/

		GameObject circleBlue = m_ParentWorld->GetGameObject(m_ComponentManager->GetEntityByComponentArrayIndex<CircleCollider>(0));
		GameObject circleRed = m_ParentWorld->GetGameObject(m_ComponentManager->GetEntityByComponentArrayIndex<CircleCollider>(1));

		GameObject plane = m_ParentWorld->GetGameObject(m_ComponentManager->GetEntityByComponentArrayIndex<PlaneCollider>(0));

		auto& circleColl1 = circleBlue.GetComponent<CircleCollider>();
		auto& circleColl2 = circleRed.GetComponent<CircleCollider>();
		auto& planeColl = plane.GetComponent<PlaneCollider>();

		if (circleColl1.TestCollision(circleBlue.GetTransform(), planeColl, plane.GetTransform()).HasCollision)
		{
			std::cout << "Collision with plane" << deltaTime.asSeconds() << "\n";
		}
		if (circleColl1.TestCollision(circleBlue.GetTransform(), circleColl2, circleRed.GetTransform()).HasCollision)
		{
			//std::cout << "Collision with circle" << deltaTime.asSeconds() << "\n";
		}
	}
};