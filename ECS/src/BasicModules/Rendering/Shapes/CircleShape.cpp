#include "CircleShape.h"

Basic::CircleShape::CircleShape()
	: m_SFMLCircle(sf::CircleShape()), m_Offset(sf::Vector2f(0.0f, 0.0f)), m_Rotation(0.0f)
{ }

Basic::CircleShape::CircleShape(float radius, size_t pointsCount)
	: m_SFMLCircle(sf::CircleShape(radius, pointsCount)), m_Offset(sf::Vector2f(0.0f, 0.0f)), m_Rotation(0.0f)
{
	m_SFMLCircle.setOrigin(sf::Vector2f(radius, radius));
}

void Basic::CircleShape::SetFillColor(const sf::Color& color)
{
	m_SFMLCircle.setFillColor(color);
}

sf::Color Basic::CircleShape::GetFillColor() const
{
	return m_SFMLCircle.getFillColor();
}

void Basic::CircleShape::SetOutlineColor(const sf::Color& color)
{
	m_SFMLCircle.setOutlineColor(color);
}

sf::Color Basic::CircleShape::GetOutlineColor() const
{
	return m_SFMLCircle.getOutlineColor();
}

void Basic::CircleShape::SetOffset(sf::Vector2f offset)
{
	m_Offset = offset;
	m_SFMLCircle.setPosition(m_Offset);
}

sf::Vector2f Basic::CircleShape::GetOffset() const
{
	return m_Offset;
}

void Basic::CircleShape::SetRotation(float rotation)
{
	m_Rotation = rotation;
	m_SFMLCircle.setRotation(rotation);
}

float Basic::CircleShape::GetRotation() const
{
	return m_Rotation;
}

Basic::AABB Basic::CircleShape::GetGlobalAABB(const Transform& transform) const
{
	AABB aabb;

	aabb.minPoint = GetGlobalPosition(transform)
		- sf::Vector2f(m_SFMLCircle.getRadius(), m_SFMLCircle.getRadius());

	aabb.maxPoint = GetGlobalPosition(transform)
		+ sf::Vector2f(m_SFMLCircle.getRadius(), m_SFMLCircle.getRadius());

	return aabb;
}


void Basic::CircleShape::SetOutlineThickness(float thickness)
{
	m_SFMLCircle.setOutlineThickness(thickness);
}

float Basic::CircleShape::GetOutlineThickness() const
{
	return m_SFMLCircle.getOutlineThickness();
}

sf::Vector2f Basic::CircleShape::GetGlobalPosition(const Transform& transform) const
{
	return m_SFMLCircle.getPosition() + transform.getPosition();
}
