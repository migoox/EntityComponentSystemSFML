#include "PhysicsEngine.h"

Basic::PhysicsEngine Basic::PhysicsEngine::s_Instance;

Basic::PhysicsEngine::PhysicsEngine()
	: m_TopView(true), m_GravityAccelerationValue(500.0f), m_GravityDirection(sf::Vector2f())
{ }

void Basic::PhysicsEngine::ISetAsTopView()
{
	m_TopView = true;
	m_GravityDirection = sf::Vector2f();
}

void Basic::PhysicsEngine::ISetAsSideView()
{
	m_TopView = false;
	m_GravityDirection = sf::Vector2f(0.0f, 1.0f);
}

bool Basic::PhysicsEngine::IsTopViewToggled()
{
	return s_Instance.m_TopView;
}

bool Basic::PhysicsEngine::IsSideViewToggled()
{
	return !s_Instance.m_TopView;
}

void Basic::PhysicsEngine::ISetGravityAccelerationValue(float acc)
{
	m_GravityAccelerationValue = acc;
}
void Basic::PhysicsEngine::ISetGravityAccelerationDirection(sf::Vector2f direction)
{
	if (m_TopView)
		direction = sf::Vector2f();
	
	m_GravityDirection = direction;
}

void Basic::PhysicsEngine::SetGravityAccelerationDirection(sf::Vector2f direction)
{
	s_Instance.ISetGravityAccelerationDirection(direction);
}

float Basic::PhysicsEngine::GravityAccelerationValue()
{
	return s_Instance.m_GravityAccelerationValue;
}

void Basic::PhysicsEngine::SetAsTopView()
{
	s_Instance.ISetAsTopView();
}

void Basic::PhysicsEngine::SetAsSideView()
{
	s_Instance.ISetAsSideView();
}

void Basic::PhysicsEngine::SetGravityAccelerationValue(float acc)
{
	s_Instance.ISetGravityAccelerationValue(acc);
}

sf::Vector2f Basic::PhysicsEngine::GravityAccelerationDirection()
{
	return s_Instance.m_GravityDirection;
}
