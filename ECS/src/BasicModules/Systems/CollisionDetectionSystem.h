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

class CollisionDetectionSystem : public ECSSystem
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

		for (size_t i = 0; i < m_GameObjects.size(); i++)
		{
			for (size_t j = i + 1; j < m_GameObjects.size(); j++)
			{
				auto& gameObject1 = m_GameObjects.at(i);
				auto& gameObject2 = m_GameObjects.at(j);

				if (gameObject1.IsActive() && gameObject2.IsActive())
				{
					auto& collider1 = gameObject1.GetComponent<Collider>();
					auto& collider2 = gameObject1.GetComponent<Collider>();

					auto& transform1 = gameObject1.GetTransform();
					auto& transform2 = gameObject1.GetTransform();

					if (collider1.Item == nullptr || collider2.Item == nullptr) continue;

					CollisionPoints collPoints = collider1.Item->TestCollision(
						transform1, collider2.Item, transform2);
				}
			}
		}

		/*
		auto circles = m_ComponentManager->ArrayData<CircleCollider>();
		size_t circleCount = m_ComponentManager->Size<CircleCollider>();

		auto planes = m_ComponentManager->ArrayData<PlaneCollider>();
		size_t planeCount = m_ComponentManager->Size<PlaneCollider>();

		// circle vs circle
		for (size_t i = 0; i < circleCount; i++)
		{
			for (size_t j = i + 1; j < circleCount; j++)
			{
				GameObject firstGameObject = m_ParentWorld->GetGameObject(
					m_ComponentManager->GetEntityByComponentArrayIndex<CircleCollider>(i));
				GameObject secondGameObject = m_ParentWorld->GetGameObject(
					m_ComponentManager->GetEntityByComponentArrayIndex<CircleCollider>(j));

				if (firstGameObject.IsActive() && secondGameObject.IsActive())
				{
					auto& first = circles[i];
					auto& second = circles[j];
					auto& transFirst = firstGameObject.GetTransform();
					auto& transSecond = secondGameObject.GetTransform();

					CollisionPoints collPoints = first.TestCollision(transFirst, second, transSecond);

					if (collPoints.HasCollision)
						collision = true;
				}
			}
		}

		// circle vs plane
		for (size_t i = 0; i < circleCount; i++)
		{
			for (size_t j = 0; j < planeCount; j++)
			{
				GameObject firstGameObject = m_ParentWorld->GetGameObject(
					m_ComponentManager->GetEntityByComponentArrayIndex<CircleCollider>(i));
				GameObject secondGameObject = m_ParentWorld->GetGameObject(
					m_ComponentManager->GetEntityByComponentArrayIndex<PlaneCollider>(j));

				if (firstGameObject.IsActive() && secondGameObject.IsActive())
				{
					auto& first = circles[i];
					auto& second = planes[j];
					auto& transFirst = firstGameObject.GetTransform();
					auto& transSecond = secondGameObject.GetTransform();

					CollisionPoints collPoints = first.TestCollision(transFirst, second, transSecond);

					if (collPoints.HasCollision)
						collision = true;
				}
			}
		}
		

		// circle vs rectangle 

		// circle vs convex

		// plane vs plane
		for (size_t i = 0; i < planeCount; i++)
		{
			for (size_t j = i + 1; j < planeCount; j++)
			{
				GameObject firstGameObject = m_ParentWorld->GetGameObject(
					m_ComponentManager->GetEntityByComponentArrayIndex<PlaneCollider>(i));
				GameObject secondGameObject = m_ParentWorld->GetGameObject(
					m_ComponentManager->GetEntityByComponentArrayIndex<PlaneCollider>(j));

				if (firstGameObject.IsActive() && secondGameObject.IsActive())
				{
					auto& first = planes[i];
					auto& second = planes[j];
					auto& transFirst = firstGameObject.GetTransform();
					auto& transSecond = secondGameObject.GetTransform();

					CollisionPoints collPoints = first.TestCollision(transFirst, second, transSecond);

					if (collPoints.HasCollision)
						collision = true;
				}
			}
		}

		// plane vs rectangle

		// plane vs convex

		// rectangle vs rectangle

		// rectangle vs convex

		// convex vs convex
		*/
		if (collision)
			std::cout << "COLLISION DETECTED" << deltaTime.asSeconds() << std::endl;
	}
};