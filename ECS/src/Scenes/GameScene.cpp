#include "GameScene.h"
#include "../BasicModules/Tools/Timer.h"

GameScene::~GameScene()
{
}

void GameScene::OnEnter()
{
	// 1. Add systems (order can be crucial in some cases)
		// your systems here
	AddSystem<InputSystem>();
	AddSystem<GrabbableElementsSystem>();

		// default systems
	AddSystem<PhysicsSystem>();
	AddSystem<CollisionSystem>();
	AddSystem<AnimatorSystem>();
	AddSystem<RendererSystem>();

	// 2. Init World Systems
	InitWorldSystems();

	// 3. Create GameObjects
	m_Test = Instantiate();

	PolygonCollider* coll = static_cast<PolygonCollider*>(m_Test.AddComponent<Collider>(
		new PolygonCollider({
		sf::Vector2f(100.0f, 50.0f),
		sf::Vector2f(500.0f, 200.0f),
		sf::Vector2f(400.0f, 300.0f),
		sf::Vector2f(90.0f, 100.0f) })).Item);

	coll->MoveCollider(sf::Vector2f(50.0f, 50.0f));
	
	m_Test.GetTransform().setPosition(200.0f, 200.0f);
	m_Test.GetTransform().setRotation(50.0f);
}

void GameScene::Update()
{
	const sf::Time& deltaTime = Game::DeltaTime();
	m_Test.GetComponent<Collider>().Item->DrawOnceOnVisualGizmos(m_Test.GetTransform());

	CircleShape center(2.0f);
	center.setFillColor(sf::Color::Green);
	center.setOrigin(1.0f, 1.0f);
	center.setPosition(m_Test.GetTransform().getPosition());

	Basic::VisualGizmos::DrawOnce(center);

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
