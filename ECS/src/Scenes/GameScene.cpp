#include "GameScene.h"
#include "../BasicModules/Tools/Timer.h"

GameScene::~GameScene()
{
}

void GameScene::OnEnter()
{
	// 1. Add systems (order can be crucial in some cases)
		// your systems here
	AddSystem<SpawnSystem>();
	AddSystem<InputSystem>();
	AddSystem<GrabbableElementsSystem>();

		// default systems
		
			// physics
	AddSystem<LinearFrictionSystem>();
	AddSystem<RotationalFrictionSystem>();
	AddSystem<LinearMovementSystem>();
	AddSystem<RotationalMovementSystem>();
	AddSystem<CollisionSystem>();

			// rendering
	AddSystem<ShapeRendererSystem>();

			// animation
	AddSystem<AnimatorSystem>();

	// 2. Init World Systems
	InitWorldSystems();

	// 3. Create GameObjects


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
