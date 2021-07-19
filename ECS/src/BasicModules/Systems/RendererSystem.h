#pragma once
#include "../ECS.h"
#include "../Components.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;

class RendererSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Constitutive);

	}

	void Render(sf::RenderTarget& target) override
	{
		// convex shape
		auto convexShapeArr = m_ComponentManager->ArrayData<ConvexShape>();
		if (convexShapeArr != nullptr)
		{
			for (size_t i = 0; i < m_ComponentManager->Size<ConvexShape>(); i++)
			{
				GameObject gameObject = m_ParentWorld->GetGameObject(m_ComponentManager->GetEntityByComponentArrayIndex<ConvexShape>(i));

				if (gameObject.IsVisible() && gameObject.IsActive())
				{
					auto& transform = m_ComponentManager->GetComponent<Transform>(gameObject.ThisEntity);

					target.draw(*convexShapeArr, transform.getTransform());
				}
				convexShapeArr++;
			}
		}

		// rectangle shape
		auto rectShapeArr = m_ComponentManager->ArrayData<RectangleShape>();
		if (rectShapeArr != nullptr)
		{
			for (size_t i = 0; i < m_ComponentManager->Size<RectangleShape>(); i++)
			{
				GameObject gameObject = m_ParentWorld->GetGameObject(m_ComponentManager->GetEntityByComponentArrayIndex<RectangleShape>(i));

				if (gameObject.IsVisible() && gameObject.IsActive())
				{
					auto& transform = m_ComponentManager->GetComponent<Transform>(gameObject.ThisEntity);

					target.draw(*rectShapeArr, transform.getTransform());
				}
				rectShapeArr++;
			}
		}

		// circle shape
		auto circleShapeArr = m_ComponentManager->ArrayData<CircleShape>();
		if (circleShapeArr != nullptr)
		{
			for (size_t i = 0; i < m_ComponentManager->Size<CircleShape>(); i++)
			{
				GameObject gameObject = m_ParentWorld->GetGameObject(m_ComponentManager->GetEntityByComponentArrayIndex<CircleShape>(i));

				if (gameObject.IsVisible() && gameObject.IsActive())
				{
					auto& transform = m_ComponentManager->GetComponent<Transform>(gameObject.ThisEntity);

					target.draw(*circleShapeArr, transform.getTransform());
				}
				circleShapeArr++;
			}
		}

		// sprites
		auto spriteArr = m_ComponentManager->ArrayData<Sprite>();
		if (spriteArr != nullptr)
		{
			for (size_t i = 0; i < m_ComponentManager->Size<Sprite>(); i++)
			{
				GameObject gameObject = m_ParentWorld->GetGameObject(m_ComponentManager->GetEntityByComponentArrayIndex<Sprite>(i));

				if (gameObject.IsVisible() && gameObject.IsActive())
				{
					auto& transform = m_ComponentManager->GetComponent<Transform>(gameObject.ThisEntity);

					target.draw(*spriteArr, transform.getTransform());
				}
				spriteArr++;
			}
		}
	}
};