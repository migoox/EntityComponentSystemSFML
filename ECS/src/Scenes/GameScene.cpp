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

	Collider& coll = m_Test.AddComponent<Collider>(new PolygonCollider(
		{
			sf::Vector2f(0.0f, 0.0f),
			sf::Vector2f(300.0f, 100.0f),
			sf::Vector2f(200.0f, 200.0f),
			sf::Vector2f(10.0f, 50.0f)
		}));


	//coll.Item->RotateCollider(50.0f);
	coll.Item->MoveCollider(sf::Vector2f(30.0f, 30.0f));
	
	m_Test.GetTransform().setPosition(400.0f, 400.0f);
}

void GameScene::Update()
{
	const sf::Time& deltaTime = Game::DeltaTime();

	m_Test.GetComponent<Collider>().Item->DrawOnceOnVisualGizmos(m_Test.GetTransform());

	CircleShape center(1.0f);
	center.setFillColor(sf::Color::Green);
	center.setOrigin(0.5f, 0.5f);
	center.setPosition(m_Test.GetTransform().getPosition());

	Basic::VisualGizmos::DrawOnce(center);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_Test.GetTransform().rotate(-20.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_Test.GetTransform().rotate(20.0f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_Test.GetComponent<Collider>().Item->RotateCollider(-20.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_Test.GetComponent<Collider>().Item->RotateCollider(20.0f);


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
