#include "GameScene.h"

GameScene::~GameScene()
{
}

void GameScene::OnEnter()
{
	srand((unsigned)time(NULL));

	// 1. Add systems
	AddSystem<InputSystem>();
	AddSystem<MotionSystem>();
	AddSystem<RendererSystem>();

	// 2. Init World(systems)
	InitWorld();

	// 3. Create GameObjects

}

void GameScene::Update()
{
	sf::Time deltaTime = Basic::Game::DeltaTime();

	UpdateWorld(deltaTime);
}

void GameScene::OnExit()
{

}

void GameScene::UpdateEvents(const sf::Event& event)
{
}

void GameScene::Render(sf::RenderTarget& target)
{
	RenderWorld(target);
}
