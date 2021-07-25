#pragma once
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Scene.h"
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

		sf::Vector2f IMouseWorldPosition();

	public:
		static void ReloadWindow();

		static void SetScene(Scene* scene);

		static void Run();

		static const sf::Time& DeltaTime();

		static sf::Vector2u WindowSize();

		static sf::Vector2f MouseWorldPosition();

		sf::VertexArray temp1;
		sf::VertexArray temp2;
		sf::VertexArray temp3;

		static void TempDraw1(sf::VertexArray v) { s_Instance.temp1 = v; }
		static void TempDraw2(sf::VertexArray v) { s_Instance.temp2 = v; }
		static void TempDraw3(sf::VertexArray v) { s_Instance.temp3 = v; }
	};
}