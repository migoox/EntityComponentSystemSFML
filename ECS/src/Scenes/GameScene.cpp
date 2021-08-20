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
	m_Test2 = Instantiate();

	auto& coll1 = m_Test2.AddComponent<Collider>(new PolygonCollider(
	{
		sf::Vector2f(0.0f, 0.0f),
		sf::Vector2f(300.0f, 100.0f),
		sf::Vector2f(200.0f, 200.0f),
		sf::Vector2f(10.0f, 50.0f)
	}));

	coll1->Solve = false;

	//coll1.Item->RotateCollider(50.0f);
	//coll1->MoveCollider(sf::Vector2f(30.0f, 30.0f));

	m_Test2.GetTransform().setPosition(200.0f, 200.0f);


	m_Test = Instantiate();


	auto& coll2 = m_Test.AddComponent<Collider>(new RectangleCollider(sf::Vector2f(200.0f, 100.0f)));
	coll2->Solve = false;

	//coll1.Item->RotateCollider(50.0f);
	//coll2->MoveCollider(sf::Vector2f(30.0f, 30.0f));
	
	m_Test.GetTransform().setPosition(400.0f, 400.0f);
}

void GameScene::Update()
{
	const sf::Time& deltaTime = Game::DeltaTime();

	m_Test.GetComponent<Collider>().Item->DrawOnceOnVisualGizmos(m_Test.GetTransform());
	m_Test2.GetComponent<Collider>().Item->DrawOnceOnVisualGizmos(m_Test2.GetTransform());

	/*CircleShape center(1.0f);
	center.setFillColor(sf::Color::Green);
	center.setOrigin(0.5f, 0.5f);
	center.setPosition(m_Test.GetTransform().getPosition());/*

	//Basic::VisualGizmos::DrawOnce(center);

	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_Test.GetTransform().rotate(-20.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_Test.GetTransform().rotate(20.0f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_Test.GetComponent<Collider>().Item->RotateCollider(-20.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_Test.GetComponent<Collider>().Item->RotateCollider(20.0f);
		*/

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_Test2.GetTransform().move(0.0f, -200.0f * deltaTime.asSeconds());
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_Test2.GetTransform().move(0.0f, 200.0f * deltaTime.asSeconds());
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		m_Test2.GetTransform().move(-200.0f * deltaTime.asSeconds(), 0.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m_Test2.GetTransform().move(200.0f * deltaTime.asSeconds(), 0.0f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_Test2.GetTransform().rotate(-2.5f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_Test2.GetTransform().rotate(2.5f);

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
