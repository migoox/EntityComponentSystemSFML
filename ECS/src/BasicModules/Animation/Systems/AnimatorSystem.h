#include "../../ECS.h"
#include "../../Components.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;

using Basic::Sprite;
using Basic::Animator;

class AnimatorSystem : public ECSSystem
{
private:

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<Sprite>();
		AddToSignature<Animator>();
	}

	void Update(const sf::Time& deltaTime) override
	{
		for (auto& gameObject : m_GameObjects)
		{
			auto& sprite = gameObject.GetComponent<Sprite>();
			auto& animator = gameObject.GetComponent<Animator>();

			animator.UpdateAnimations(deltaTime);

			sprite.setTextureRect(animator.GetCurrentFrameRect());
		}
	}
};