#pragma once
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Scenes/Scene.h"
#include "StateSystem/StateMachine.h"

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

	public:
		static void ReloadWindow();

		static void SetScene(Scene* scene);

		static void Run();

		static const sf::Time& DeltaTime();

		static sf::Vector2u WindowSize();
	};
}