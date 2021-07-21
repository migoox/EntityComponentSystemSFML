#include "GameScene.h"

GameScene::~GameScene()
{
}

void GameScene::OnEnter()
{
	// 1. Add systems
	// AddSystem<InputSystem>();
	// AddSystem<MotionSystem>();
	AddSystem<PlayerSystem>();
	AddSystem<AnimatorSystem>();
	AddSystem<RendererSystem>();

	// 2. Init World Systems
	InitWorldSystems();

	// 3. Create GameObjects
	GameObject player = Instantiate();

	auto& rigidBody = player.AddComponent<RigidBody>(RigidBody());

	auto& sprite = player.AddComponent<Sprite>(Sprite());
	sprite.setTexture(*ResourceManager::TextureAcquire("resources/sample.png"));

	auto& animator = player.AddComponent<Animator>(Animator(sprite.getTexture()->getSize()));
	animator.AddAnimation("FRONT_WALK", Animation(sf::Vector2u(0, 0), sf::Vector2u(16, 16), 4, 0.16f));
	animator.AddAnimation("BACK_WALK", Animation(sf::Vector2u(0, 16), sf::Vector2u(16, 16), 4, 0.16f));
	animator.AddAnimation("HORIZONTAL_WALK", Animation(sf::Vector2u(0, 32), sf::Vector2u(16, 16), 2, 0.16f));
	animator.AddAnimation("KICK", Animation(sf::Vector2u(16, 32), sf::Vector2u(16, 16), 1, 0.18f));
	animator.AddAnimation("IDLE", Animation(sf::Vector2u(0, 0), sf::Vector2u(16, 16), 1, 0.5f));

	player.GetTransform().setOrigin(8, 8);
	player.GetTransform().setScale(6.0f, 6.0f);
}

void GameScene::Update()
{
	sf::Time deltaTime = Game::DeltaTime();

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
