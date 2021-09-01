#include "GameScene.h"
#include "../BasicModules/Tools/Timer.h"

GameScene::~GameScene()
{
}

void GameScene::OnEnter()
{
	// 1. Add systems (order can be crucial in some cases)
		// your systems here
	//AddSystem<SpawnSystem>();
	//AddSystem<InputSystem>();
	//AddSystem<GrabbableElementsSystem>();

		// default systems
	AddSystem<PhysicsSystem>();
	AddSystem<CollisionSystem>();
	AddSystem<AnimatorSystem>();
	AddSystem<ShapeRendererSystem>();

	// 2. Init World Systems
	InitWorldSystems();

	// 3. Create GameObjects
	/*test = Instantiate();

	auto& shape = test.AddComponent<Basic::Shape>(
		new Basic::PolygonShape(
			{sf::Vector2f(0.0f, 0.0f), sf::Vector2f(100.0f, 60.0f), sf::Vector2f(40.0f, 100.0f), sf::Vector2f(-10.0f, 10.0f) }
	));
	shape->SetFillColor(sf::Color(0.0f, 200.0f, 100.0f, 200.0f));

	test.GetTransform().setPosition(sf::Vector2f(500.0f, 100.0f));

	shape->SetOffset(sf::Vector2f(0.0f, 100.0f));
	shape->SetOffset(sf::Vector2f(0.0f, 200.0f));
	shape->SetRotation(50.0f);

	test.GetTransform().setRotation(100.0);*/

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
	//auto& shape = test.GetComponent<Basic::Shape>();

	//Basic::AABB aabb = shape->GetGlobalAABB(test.GetTransform());
	
	
	/*sf::RectangleShape rect;
	rect.setSize(aabb.maxPoint - aabb.minPoint);
	rect.setOrigin((aabb.maxPoint - aabb.minPoint) / 2.0f);
	rect.setPosition((aabb.maxPoint - aabb.minPoint) / 2.0f + aabb.minPoint);
	rect.setFillColor(sf::Color(30.0f, 60.0f, 0.0f, 150.0f));
	target.draw(rect);

	sf::CircleShape trans(2.0f);
	trans.setOrigin(2.0f, 2.0f);
	trans.setPosition(test.GetTransform().getPosition());
	trans.setFillColor(sf::Color::Red);
	target.draw(trans);

	sf::CircleShape circlePos(2.0f);
	circlePos.setOrigin(2.0f, 2.0f);
	circlePos.setPosition(shape->GetGlobalPosition(test.GetTransform()));
	circlePos.setFillColor(sf::Color::Blue);
	target.draw(circlePos);
	*/
	RenderWorld(target);
}
