#pragma once
#include <fstream>

#include "StateSystem/StateMachine.h"

#include "Scenes/GameScene.h"

// fixes scaling to 4k
#ifdef USING_WINDOWS
SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
#endif

namespace Framework {
	class Game : public StateMachine
	{
	private:
		sf::RenderWindow m_Window;
		sf::Event m_Event;
		sf::Clock m_Timer;

		static Scene* _CurrentScene;

		static sf::Time _DeltaTime;

		static Game* _Instance;

	private:
		void InitWindow();
		void UpdateWindowEvents();
		void Update();
		void Render();

	public:
		// Constructors & Destructor
		Game();
		~Game();

		// Functionality
		void ReloadWindow();

		void SetScene(Scene* scene);
		void Run();

		static Game *GetInstance();
	};
}