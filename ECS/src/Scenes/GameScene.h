#pragma once
#include "Scene.h"
#include "../Game.h"

class GameScene : public Framework::Scene
{
public:
	using Framework::Scene::Scene;
	~GameScene();

	void OnEnter() override;
	void Update() override;
	void OnExit() override;

	void UpdateEvents(const sf::Event& event) override;
	void Render() override;
};