#include "GameScene.h"

GameScene::~GameScene()
{
}

void GameScene::OnEnter()
{
	// 1. Add systems (order can be crucial in some cases)
	AddSystem<InputSystem>();
	AddSystem<PlayerSystem>();
	AddSystem<MotionSystem>();
	AddSystem<AnimatorSystem>();
	AddSystem<RendererSystem>();

	// 2. Init World Systems
	InitWorldSystems();

	// 3. Create GameObjects
	GameObject player = Instantiate(); // Instantiate(player) is also allowed

	// adding important components
	player.AddComponent<Joystick>(Joystick());
	player.AddComponent<Stats>(Stats());
	player.AddComponent<RigidBody>(RigidBody());

	auto& sprite = player.AddComponent<Sprite>(Sprite(*ResourceManager::TextureAcquire("resources/sample.png")));

	// changing transform ( transform is special component, which cannot be added or deleted )
	player.GetTransform().setOrigin(8, 8);
	player.GetTransform().setScale(6.0f, 6.0f);

	// adding animations to our player
	auto& animator = player.AddComponent<Animator>(Animator(sprite.getTexture()->getSize()));
	animator.AddAnimation("FRONT_WALK", Animation(sf::Vector2u(0, 0), sf::Vector2u(16, 16), 4, 0.16f));
	animator.AddAnimation("BACK_WALK", Animation(sf::Vector2u(0, 16), sf::Vector2u(16, 16), 4, 0.16f));
	animator.AddAnimation("HORIZONTAL_WALK", Animation(sf::Vector2u(0, 32), sf::Vector2u(16, 16), 2, 0.16f));
	animator.AddAnimation("IDLE", Animation(sf::Vector2u(0, 0), sf::Vector2u(16, 16), 1, 0.5f));
}

void GameScene::Update()
{
	const sf::Time& deltaTime = Game::DeltaTime();

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
