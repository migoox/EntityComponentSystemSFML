#include "PlaneCollider.h"

sf::Vector2f Basic::PlaneCollider::TranslateRelativePointToGlobal(sf::Vector2f point, const Transform& trans) const
{
	sf::Vector2f scale = trans.getScale();
	float angle = trans.getRotation() * 3.141592653f / 180.0f;
	float colliderAngle = m_ColliderRotation * 3.141592653f / 180.0f;

	point = sf::Vector2f(
		point.x * std::cos(colliderAngle) - point.y * std::sin(colliderAngle),
		point.x * std::sin(colliderAngle) + point.y * std::cos(colliderAngle));

	sf::Vector2f global = point + m_ColliderDisplacement;

	global.x *= scale.x;
	global.y *= scale.y;

	global = sf::Vector2f(
		global.x * std::cos(angle) - global.y * std::sin(angle),
		global.x * std::sin(angle) + global.y * std::cos(angle));

	global += trans.getPosition();


	return global;
}

float Basic::PlaneCollider::Distance() const
{
	return m_Distance;
}

sf::Vector2f Basic::PlaneCollider::GetGlobalCenterOfGravity(const Transform& trans) const
{
	return TranslateRelativePointToGlobal(sf::Vector2f(0.0f, 0.0f), trans);
}

float Basic::PlaneCollider::GetMomentOfInertia(const RigidBody& rb) const
{
	return 1.0f / 12.0f * float(std::pow(m_Distance, 2)) * rb.Mass;
}

void Basic::PlaneCollider::DrawOnceOnVisualGizmos(const Transform& trans) const
{
	Line line(GetGlobalAPoint(trans), GetGlobalBPoint(trans), 4.0f, sf::Color::Blue);
	CircleShape gCenter(2.0f);
	gCenter.setFillColor(sf::Color::Red);
	gCenter.setOrigin(1.0f, 1.0f);
	gCenter.setPosition(GetGlobalCenterOfGravity(trans));

	Basic::VisualGizmos::DrawOnce(line);
	Basic::VisualGizmos::DrawOnce(gCenter);
}

void Basic::PlaneCollider::MoveCollider(sf::Vector2f displacement)
{
	m_ColliderDisplacement += displacement;
}

void Basic::PlaneCollider::RotateCollider(float angle)
{
	m_ColliderRotation += angle;
}

sf::Vector2f Basic::PlaneCollider::GetGlobalAPoint(const Transform& trans) const
{
	return TranslateRelativePointToGlobal(sf::Vector2f(-m_Distance / 2.0f, 0.0f), trans);
}

sf::Vector2f Basic::PlaneCollider::GetGlobalBPoint(const Transform& trans) const
{
	return TranslateRelativePointToGlobal(sf::Vector2f(m_Distance / 2.0f, 0.0f), trans);
}

sf::Vector2f Basic::PlaneCollider::FindFurthestPointInDirection(const Transform& transform, sf::Vector2f direction) const
{
	return sf::Vector2f();
}
