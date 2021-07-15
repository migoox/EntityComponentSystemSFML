#include "Game.h"

Framework::Scene* Framework::Game::_CurrentScene;
sf::Time Framework::Game::_DeltaTime;
Framework::Game* Framework::Game::_Instance;

void Framework::Game::InitWindow()
{
	std::ifstream file;
	file.open("Config/window.ini");

	if (file.is_open())
	{
		unsigned int x;
		unsigned int y;
		bool verticalSync;
		unsigned int framerateLimit;

		// getting size of window
		file >> x >> y;

		// getting vertical sync flag
		file >> verticalSync;

		// framerateLimit
		file >> framerateLimit;

		m_Window.create(sf::VideoMode(x, y), "Game");
		m_Window.setVerticalSyncEnabled(verticalSync);
		m_Window.setFramerateLimit(framerateLimit);

		file.close();
		return; // end func
	}

	// create default window in case there is no config file
	m_Window.create(sf::VideoMode(800, 600), "Game");
}

void Framework::Game::ReloadWindow()
{

}

void Framework::Game::UpdateWindowEvents()
{
	// if close button is clicked, close app
	if (m_Event.type == sf::Event::Closed)
		m_Window.close();

	_CurrentScene->UpdateEvents(m_Event);
}

void Framework::Game::Update()
{
	// update current scene
	UpdateStateMachine();
}

void Framework::Game::Render()
{
	// render current scene
	_CurrentScene->Render();
}

Framework::Game::Game()
{
	// init window
	InitWindow();

	// set start scene, program won't run without active scene
	SetScene(new GameScene(&m_Window));

	// setting singleton 
	_Instance = this;
}

Framework::Game::~Game()
{

}

void Framework::Game::SetScene(Scene* scene)
{
	// setting current scene
	_CurrentScene = scene;
	
	// launching scene
	LaunchState(_CurrentScene);
}

void Framework::Game::Run()
{
	// start counting
	m_Timer.restart();
	while (m_Window.isOpen())
	{
		// if there is no scene active, close window
		if (!IsMachineActive())
		{
			m_Window.close();
		}

		// update delta time between frames
		_DeltaTime = m_Timer.restart();

		// check sfml events
		while (m_Window.pollEvent(m_Event))
		{
			UpdateWindowEvents();
		}

		// update game
		Update();

		// clear window
		m_Window.clear();

		// render stuff
		Render();

		// display stuff on window
		m_Window.display();
	}
}

Framework::Game* Framework::Game::GetInstance()
{
	return _Instance;
}
