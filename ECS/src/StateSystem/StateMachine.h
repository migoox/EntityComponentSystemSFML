#pragma once
#include "State.h"

namespace Framework
{
	class StateMachine
	{
	private:
		State* m_CurrentState;

	public:
		StateMachine();
		~StateMachine();

		void LaunchState(State* state);
		void UpdateStateMachine();
		void EndCurrentState();

		bool IsMachineActive();
	};
}