#include "Game.h"

Basic::Scene* Basic::Game::s_CurrentScene;
sf::Time Basic::Game::s_DeltaTime;
Basic::Game Basic::Game::s_Instance;

void Basic::Game::InitWindow()
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
	m_Window.setFramerateLimit(60);
}

void Basic::Game::IReloadWindow()
{

}

void Basic::Game::UpdateWindowEvents()
{
	// if close button is clicked, close app
	if (m_Event.type == sf::Event::Closed)
		m_Window.close();

	s_CurrentScene->UpdateEvents(m_Event);
}

void Basic::Game::Update()
{
	// update current scene
	UpdateStateMachine();
}

void Basic::Game::Render()
{
	// render current scene
	s_CurrentScene->Render(m_Window);
}

Basic::Game::Game()
{
	// init window
	InitWindow();
}

void Basic::Game::ISetScene(Scene* scene)
{
	// setting current scene
	s_CurrentScene = scene;
	
	// launching scene
	LaunchState(s_CurrentScene);
}

void Basic::Game::IRun()
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
		s_DeltaTime = m_Timer.restart();

		// check sfml events
		while (m_Window.pollEvent(m_Event))
		{
			UpdateWindowEvents();
		}

		// update game
		Update();

		// clear window
		m_Window.clear(sf::Color::White);

		// render stuff
		Render();

		// display stuff on window
		m_Window.display();
	}
}

void Basic::Game::ReloadWindow()
{
	s_Instance.IReloadWindow();
}

void Basic::Game::SetScene(Scene* scene)
{
	s_Instance.ISetScene(scene);
}

void Basic::Game::Run()
{
	s_Instance.IRun();
}

const sf::Time& Basic::Game::DeltaTime()
{
	return s_DeltaTime;
}

sf::Vector2u Basic::Game::WindowSize()
{
	return s_Instance.m_Window.getSize();
}
