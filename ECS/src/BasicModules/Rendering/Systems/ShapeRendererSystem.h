#pragma once
#include "../../ECS.h"
#include "../../Components.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;

class ShapeRendererSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<Basic::Shape>();
	}

	void Render(sf::RenderTarget& target) override
	{
		using Basic::Shape;

		for (auto& gameObject : m_GameObjects)
		{
			auto& transform = gameObject.GetTransform();
			auto& shape = gameObject.GetComponent<Shape>();
			
			target.draw(*shape.Drawing, transform.getTransform());
		}
	}
};