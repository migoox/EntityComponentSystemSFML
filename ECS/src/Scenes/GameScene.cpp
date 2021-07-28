#include "GameScene.h"
#include "../BasicModules/Timer.h"

GameScene::~GameScene()
{
}

void GameScene::OnEnter()
{
	// 1. Add systems (order can be crucial in some cases)
		// your systems here
	AddSystem<SelectionSystem>();
	AddSystem<MotionSystem>();

		// default systems
	AddSystem<PhysicsSystem>();
	AddSystem<CollisionSystem>();
	AddSystem<AnimatorSystem>();
	AddSystem<RendererSystem>();

	// 2. Init World Systems
	InitWorldSystems();

	// 3. Create GameObjects

	// first

	Collider* collider;
	RigidBody* rigidBody;
	for (size_t i = 0; i < 100; i++)
	{
		GameObject circle = Instantiate();

		auto& circleShape = circle.AddComponent<CircleShape>(CircleShape(10.0f));
		circleShape.setOrigin(10.0f, 10.0f);

		collider = &circle.AddComponent<Collider>(new CircleCollider(10.0f));
		collider->Item->Solid = false;

		rigidBody = &circle.AddComponent<RigidBody>(RigidBody());
		rigidBody->UseGravity = false;

		circle.GetTransform().setPosition(sf::Vector2f(rand() % 800, rand() % 600));

		circle.AddComponent<Selectable>(Selectable());
	}

	// left wall
	auto plane = Instantiate();
	auto& line = plane.AddComponent<Line>(Line(600.0f, 6.0f, sf::Color::Black));
	line.setOrigin(300.0f, 0.0f);

	collider = &plane.AddComponent<Collider>(new PlaneCollider(600.0f));
	collider->Item->Solid = true;

	rigidBody = &plane.AddComponent<RigidBody>(RigidBody());
	rigidBody->UseGravity = false;
	rigidBody->FreezeXAxisMovement = true;
	rigidBody->FreezeYAxisMovement = true;
	rigidBody->FreezeRotation = true;

	plane.GetTransform().setRotation(-90.0f);
	plane.GetTransform().setPosition(sf::Vector2f(0.f, 300.0f));

	// right wall
	plane = Instantiate();
	auto& line2 = plane.AddComponent<Line>(Line(600.0f, 6.0f, sf::Color::Black));
	line2.setOrigin(300.0f, 0.0f);

	collider = &plane.AddComponent<Collider>(new PlaneCollider(600.0f));
	collider->Item->Solid = true;

	rigidBody = &plane.AddComponent<RigidBody>(RigidBody());
	rigidBody->UseGravity = false;
	rigidBody->FreezeXAxisMovement = true;
	rigidBody->FreezeYAxisMovement = true;
	rigidBody->FreezeRotation = true;

	plane.GetTransform().setRotation(90.0f);
	plane.GetTransform().setPosition(sf::Vector2f(800.f, 300.0f));

	// bottom
	plane = Instantiate();
	auto& line3 = plane.AddComponent<Line>(Line(800.0f, 6.0f, sf::Color::Black));
	line3.setOrigin(400.0f, 0.0f);

	collider = &plane.AddComponent<Collider>(new PlaneCollider(800.0f));
	collider->Item->Solid = true;

	rigidBody = &plane.AddComponent<RigidBody>(RigidBody());
	rigidBody->UseGravity = true;
	rigidBody->FreezeXAxisMovement = true;
	rigidBody->FreezeYAxisMovement = true;
	rigidBody->FreezeRotation = true;

	plane.GetTransform().setPosition(sf::Vector2f(400.0f, 600.0f));

	// top
	plane = Instantiate();
	auto& line4 = plane.AddComponent<Line>(Line(800.0f, 6.0f, sf::Color::Black));
	line4.setOrigin(400.0f, 0.0f);

	collider = &plane.AddComponent<Collider>(new PlaneCollider(800.0f));
	collider->Item->Solid = true;

	rigidBody = &plane.AddComponent<RigidBody>(RigidBody());
	rigidBody->UseGravity = false;
	rigidBody->FreezeXAxisMovement = true;
	rigidBody->FreezeYAxisMovement = true;
	rigidBody->FreezeRotation = true;

	plane.GetTransform().setPosition(sf::Vector2f(400.f, 0.0f));


	plane = Instantiate();
	auto& line5 = plane.AddComponent<Line>(Line(300.0f, 6.0f, sf::Color::Black));
	line5.setOrigin(150.0f, 0.0f);

	collider = &plane.AddComponent<Collider>(new PlaneCollider(300.0f));
	collider->Item->Solid = true;

	rigidBody = &plane.AddComponent<RigidBody>(RigidBody());
	rigidBody->UseGravity = false;
	rigidBody->FreezeXAxisMovement = true;
	rigidBody->FreezeYAxisMovement = true;
	rigidBody->FreezeRotation = true;

	plane.GetTransform().setRotation(60.0f);
	plane.GetTransform().setPosition(sf::Vector2f(400.f, 300.0f));
}

void GameScene::Update()
{
	Timer timer;
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
