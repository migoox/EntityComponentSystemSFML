#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>

#include "Scene.h"
#include "StateMachine/StateMachine.h"
#include "Rendering/VisualGizmos.h"

// fixes scaling to 4k
#ifdef USING_WINDOWS
SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
#endif

// singleton Game Class
namespace Basic {
	class Game : public StateMachine
	{
	private:
		sf::Event m_Event;
		sf::Clock m_Timer;

		sf::Color m_BackgroundColor;

		sf::RenderWindow m_Window;

		static Scene* s_CurrentScene;

		static sf::Time s_DeltaTime;

		static Game s_Instance;

	private:
		// Constructors & Destructor
		Game();

		void InitWindow();
		void UpdateWindowEvents();
		void Update();
		void Render();

		// Functionality
		void IReloadWindow();
		
		void ISetScene(Scene* scene);
		void IRun();

		void ISetBackgroundColor(sf::Color color);

		sf::Vector2f IMouseWorldPosition();

	public:
		static void ReloadWindow();

		static void SetScene(Scene* scene);

		static void Run();

		static const sf::Time& DeltaTime();

		static sf::Vector2u WindowSize();

		static sf::Vector2f MouseWorldPosition();

		static void SetBackgroundColor(sf::Color color);
	};
}