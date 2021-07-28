#pragma once
#include <iostream>
#include <memory>

// basic modules
#include "../BasicModules/ECS.h"
#include "../BasicModules/Components.h"
#include "../BasicModules/Systems.h"
#include "../BasicModules/Game.h"
#include "../BasicModules/ResourceManager/ResourceManager.h"

// include external systems

// include external components
#include "../ExternalSystems/SelectionSystem.h"
#include "../ExternalSystems/MotionSystem.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;
using Basic::ResourceManager;
using Basic::Game;
using Basic::Scene;

class GameScene : public Scene
{
private:

public:
	using Scene::Scene;
	~GameScene();

	void OnEnter() override;
	void Update() override;
	void OnExit() override;

	void UpdateEvents(const sf::Event& event) override;
	void Render(sf::RenderTarget& target) override;

public:

};