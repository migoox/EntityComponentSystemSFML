#pragma once

namespace Basic {
	class State
	{
	protected:
		bool m_Quit;

	public:
		State()
			: m_Quit(false)
		{
		}

		virtual ~State() {};

		virtual void OnEnter() {};
		virtual void Update() {};
		virtual void OnExit() {};

		void End() { m_Quit = true; }

		bool IsEnded() const { return m_Quit; };

	};
}