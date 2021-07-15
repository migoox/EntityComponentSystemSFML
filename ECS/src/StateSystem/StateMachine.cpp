#include "StateMachine.h"

Framework::StateMachine::StateMachine()
	: m_CurrentState(nullptr)
{
}

Framework::StateMachine::~StateMachine()
{
	EndCurrentState();
}

void Framework::StateMachine::LaunchState(State* state)
{
	EndCurrentState();

	m_CurrentState = state;

	m_CurrentState->OnEnter();
}

void Framework::StateMachine::UpdateStateMachine()
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

void Framework::StateMachine::EndCurrentState()
{
	if (m_CurrentState != nullptr)
	{
		m_CurrentState->OnExit();
		
		delete m_CurrentState;

		m_CurrentState = nullptr;
	}
}

bool Framework::StateMachine::IsMachineActive()
{
	return m_CurrentState != nullptr;
}
