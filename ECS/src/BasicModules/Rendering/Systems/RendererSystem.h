#pragma once
#include "../../ECS.h"
#include "../../Components.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;

using namespace Basic::Components;

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
					auto& transform = gameObject.GetTransform();

					target.draw(*convexShapeArr, transform.getTransform());
				}
				convexShapeArr++;
			}
		}

		// line
		auto lineArr = m_ComponentManager->ArrayData<Line>();
		if (lineArr != nullptr)
		{
			for (size_t i = 0; i < m_ComponentManager->Size<Line>(); i++)
			{
				GameObject gameObject = m_ParentWorld->GetGameObject(m_ComponentManager->GetEntityByComponentArrayIndex<Line>(i));

				if (gameObject.IsVisible() && gameObject.IsActive())
				{
					auto& transform = gameObject.GetTransform();

					target.draw(*lineArr, transform.getTransform());
				}
				lineArr++;
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
					auto& transform = gameObject.GetTransform();

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
					auto& transform = gameObject.GetTransform();

					target.draw(*circleShapeArr, transform.getTransform());
				}
				circleShapeArr++;
			}
		}

		// vertex array
		auto vertArrArr = m_ComponentManager->ArrayData<VertexArray>();
		if (vertArrArr != nullptr)
		{
			for (size_t i = 0; i < m_ComponentManager->Size<VertexArray>(); i++)
			{
				GameObject gameObject = m_ParentWorld->GetGameObject(m_ComponentManager->GetEntityByComponentArrayIndex<VertexArray>(i));

				if (gameObject.IsVisible() && gameObject.IsActive())
				{
					auto& transform = gameObject.GetTransform();

					target.draw(*vertArrArr, transform.getTransform());
				}
				vertArrArr++;
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
					auto& transform = gameObject.GetTransform();

					target.draw(*spriteArr, transform.getTransform());
				}
				spriteArr++;
			}
		}
	}
};