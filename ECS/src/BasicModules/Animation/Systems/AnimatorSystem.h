#include "../../ECS.h"
#include "../../Components.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;


class AnimatorSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<Basic::Sprite>();
		AddToSignature<Basic::Animator>();
	}

	void Update(const sf::Time& deltaTime) override
	{
		using Basic::Sprite;
		using Basic::Animator;

		for (auto& gameObject : m_GameObjects)
		{
			auto& sprite = gameObject.GetComponent<Sprite>();
			auto& animator = gameObject.GetComponent<Animator>();

			animator.UpdateAnimations(deltaTime);

			//sprite.setTextureRect(animator.GetCurrentFrameRect());
		}
	}
};