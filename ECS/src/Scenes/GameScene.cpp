#include "GameScene.h"
#include "../BasicModules/Timer.h"

GameScene::~GameScene()
{
}

void GameScene::OnEnter()
{
	// 1. Add systems (order can be crucial in some cases)
		// your systems here
	AddSystem<TestSystem>();

		// default systems
	AddSystem<PhysicsSystem>();
	AddSystem<CollisionSystem>();
	AddSystem<AnimatorSystem>();
	AddSystem<RendererSystem>();

	// 2. Init World Systems
	InitWorldSystems();

	// 3. Create GameObjects

	// first
	GameObject circle = Instantiate();

	auto& circleShape = circle.AddComponent<CircleShape>(CircleShape(75.0f));
	circleShape.setFillColor(sf::Color(0, 0, 255, 225));
	circleShape.setOrigin(75.0f, 75.0f);

	Collider* collider = &circle.AddComponent<Collider>(new CircleCollider(75.0f));
	collider->Item->Solid = false;


	circle.GetTransform().setPosition(sf::Vector2f(100.f, 100.f));

	// second
	circle = Instantiate();

	auto& circleShape2 = circle.AddComponent<CircleShape>(CircleShape(100.0f));
	circleShape2.setFillColor(sf::Color(255, 0, 0, 225));
	circleShape2.setOrigin(100.0f, 100.0f);

	collider = &circle.AddComponent<Collider>(new CircleCollider(100.0f));
	collider->Item->Solid = false;

	circle.GetTransform().setPosition(sf::Vector2f(600.f, 300.f));

	// third
	circle = Instantiate();

	auto& circleShape3 = circle.AddComponent<CircleShape>(CircleShape(125.0f));
	circleShape3.setFillColor(sf::Color(0, 255, 0, 225));
	circleShape3.setOrigin(125.0f, 125.0f);

	collider = &circle.AddComponent<Collider>(new CircleCollider(125.0f));
	collider->Item->Solid = false;

	circle.GetTransform().setPosition(sf::Vector2f(500.f, 400.f));

	// plane
	auto plane = Instantiate();
	auto& line = plane.AddComponent<Line>(Line(500.0f, 1.0f, sf::Color::Black));
	line.setOrigin(250.0f, 0.0f);

	collider = &plane.AddComponent<Collider>(new PlaneCollider(500.0f));
	collider->Item->Solid = false;

	plane.GetTransform().setRotation(30.0f);
	plane.GetTransform().setPosition(sf::Vector2f(300.f, 400.0f));
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
