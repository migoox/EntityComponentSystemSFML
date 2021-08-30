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

	//auto& coll1 = m_Test.AddComponent<Collider>(new CircleCollider(50.0f));

	auto& coll1 = m_Test.AddComponent<Collider>(new RectangleCollider(sf::Vector2f(100.0f, 50.0f)));


	// coll1.Item->RotateCollider(50.0f);
	// coll1->MoveCollider(sf::Vector2f(30.0f, 30.0f));

	m_Test.GetTransform().setPosition(300.0f, 300.0f);


	m_Test2 = Instantiate();

	auto& coll2 = m_Test2.AddComponent<Collider>(new PolygonCollider(
		{
			sf::Vector2f(0.0f, 0.0f),
			sf::Vector2f(300.0f, 100.0f),
			sf::Vector2f(200.0f, 200.0f),
			sf::Vector2f(10.0f, 50.0f)
		}));

	//auto& coll2 = m_Test2.AddComponent<Collider>(new RectangleCollider(sf::Vector2f(100.0f, 120.0f)));
	//coll1.Item->RotateCollider(50.0f);
	//coll2->MoveCollider(sf::Vector2f(30.0f, 30.0f));
	
	m_Test2.GetTransform().setPosition(600.0f, 385.0f);

	//m_Test3 = Instantiate();

	//auto& coll3 = m_Test3.AddComponent<Collider>(new CircleCollider(60.0f));

	//m_Test3.GetTransform().setPosition(sf::Vector2f(150.0, 100.0f));

	//m_Test4 = Instantiate();

	//auto& coll4 = m_Test4.AddComponent<Collider>(new PlaneCollider(300.0f));

	//m_Test4.GetTransform().setPosition(sf::Vector2f(350.0f, 350.0f));

	//m_Test4.GetTransform().rotate(30.0f);

	// auto& rb = m_Test4.AddComponent<RigidBody>(RigidBody());
	// rb.AngleVelocity = 0.05f;
}

void GameScene::Update()
{
	const sf::Time& deltaTime = Game::DeltaTime();

	m_Test.GetComponent<Collider>()->DrawOnceOnVisualGizmos(m_Test.GetTransform());
	//m_Test.GetComponent<Collider>()->DrawAABBOnceOnVisualGizmos(m_Test.GetTransform());
	m_Test2.GetComponent<Collider>()->DrawOnceOnVisualGizmos(m_Test2.GetTransform());
	//m_Test2.GetComponent<Collider>()->DrawAABBOnceOnVisualGizmos(m_Test2.GetTransform());
	//m_Test3.GetComponent<Collider>()->DrawOnceOnVisualGizmos(m_Test3.GetTransform());
	//m_Test3.GetComponent<Collider>()->DrawAABBOnceOnVisualGizmos(m_Test3.GetTransform());
	//m_Test4.GetComponent<Collider>()->DrawOnceOnVisualGizmos(m_Test4.GetTransform());
	//m_Test4.GetComponent<Collider>()->DrawAABBOnceOnVisualGizmos(m_Test4.GetTransform());

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
