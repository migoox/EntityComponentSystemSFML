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
	AddSystem<CollisionDetectionSystem>();
	AddSystem<AnimatorSystem>();
	AddSystem<RendererSystem>();

	// 2. Init World Systems
	InitWorldSystems();

	// 3. Create GameObjects
	/*GameObject box = Instantiate();

	auto& rigidBody = box.AddComponent<RigidBody>(RigidBody());
	rigidBody.FreezeRotation = true;
	rigidBody.UseGravity = true;
	rigidBody.Mass = 50.0f;

	auto& rectShape = box.AddComponent<RectangleShape>(RectangleShape(sf::Vector2f(96.0f, 96.0f)));
	rectShape.setFillColor(sf::Color::Black);
	
	box.GetTransform().setOrigin(48.0f, 48.0f);
	box.GetTransform().setPosition(Game::WindowSize().x / 2, 48.0f);*/

	GameObject circle = Instantiate();

	auto& circleShape = circle.AddComponent<CircleShape>(CircleShape(75.0f));
	circleShape.setFillColor(sf::Color(0, 0, 255, 225));
	circleShape.setOrigin(75.0f, 75.0f);

	circle.AddComponent<Collider>(new CircleCollider(75.0f));
	circle.GetTransform().setPosition(sf::Vector2f(100.f, 100.f));

	circle = Instantiate();

	auto& circleShape2 = circle.AddComponent<CircleShape>(CircleShape(100.0f));
	circleShape2.setFillColor(sf::Color(255, 0, 0, 225));
	circleShape2.setOrigin(100.0f, 100.0f);

	circle.AddComponent<Collider>(new CircleCollider(100.0f));
	circle.GetTransform().setPosition(sf::Vector2f(600.f, 300.f));
	GameObject plane = Instantiate();

	auto& verArr = plane.AddComponent<Line>(Line(260.0f, 10.0f, sf::Color::Black));

	plane.AddComponent<Collider>(new PlaneCollider(260.0f));

	plane.GetTransform().setRotation(-20.0f);
	plane.GetTransform().setPosition(200.0f, 400.0f);
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
