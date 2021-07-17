#pragma once
#include "Scene.h"
#include "../Game.h"
#include "../ECS/ECS.h"
#include <memory>

class RendererSystem : public System
{
private:

public:
	void Init() override
	{
		AddToSignature<sf::CircleShape>();
	}

	void Update(const sf::Time& deltaTime) override
	{

	}

	void Render(sf::RenderTarget& target) override
	{
		for (auto it : m_Entities)
		{
			auto shape = m_ParentWorld->GetComponent<sf::CircleShape>(it);
			target.draw(shape);
		}
	}
};

class GameScene : public Framework::Scene
{
private:
	World world;
	std::shared_ptr<System> renderer;

public:
	using Framework::Scene::Scene;
	~GameScene();

	void OnEnter() override;
	void Update() override;
	void OnExit() override;

	void UpdateEvents(const sf::Event& event) override;
	void Render(sf::RenderTarget& target) override;

public:

};