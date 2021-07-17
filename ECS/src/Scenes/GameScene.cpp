#include "GameScene.h"

GameScene::~GameScene()
{
}

void GameScene::OnEnter()
{
	world.RegisterComponent<sf::CircleShape>();
	world.RegisterComponent<sf::Transform>();

	renderer = std::make_shared<RendererSystem>();
	world.AddSystem(renderer);

	world.Init();

	EntityHandle entity = world.CreateEntity();
	entity.AddComponent<sf::CircleShape>(sf::CircleShape(5.0f));
	entity.AddComponent<sf::Transform>(sf::Transform());

}

void GameScene::Update()
{
	world.Update(Framework::Game::DeltaTime());
}

void GameScene::OnExit()
{

}

void GameScene::UpdateEvents(const sf::Event& event)
{
}

void GameScene::Render(sf::RenderTarget& target)
{
	world.Render(target);
}
