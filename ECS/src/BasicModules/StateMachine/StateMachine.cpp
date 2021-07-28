#include "StateMachine.h"

Basic::StateMachine::StateMachine()
	: m_CurrentState(nullptr)
{
}

Basic::StateMachine::~StateMachine()
{
	EndCurrentState();
}

void Basic::StateMachine::LaunchState(State* state)
{
	EndCurrentState();

	m_CurrentState = state;

	m_CurrentState->OnEnter();
}

void Basic::StateMachine::UpdateStateMachine()
{
	if (m_CurrentState->IsEnded())
	{
		EndCurrentState();
	}
	else
	{
		m_CurrentState->Update();
	}
}

void Basic::StateMachine::EndCurrentState()
{
	if (m_CurrentState != nullptr)
	{
		m_CurrentState->OnExit();
		
		delete m_CurrentState;

		m_CurrentState = nullptr;
	}
}

bool Basic::StateMachine::IsMachineActive()
{
	return m_CurrentState != nullptr;
}
