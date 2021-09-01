#include "BoxShape.h"

sf::Vector2f Basic::BoxShape::TranslateRelativePointToGlobal(sf::Vector2f point, const Transform& trans) const
{
	sf::Vector2f scale = trans.getScale();
	float angle = trans.getRotation() * 3.141592653f / 180.0f;
	float colliderAngle = m_Rotation * 3.141592653f / 180.0f;

	point = sf::Vector2f(
		point.x * std::cos(colliderAngle) - point.y * std::sin(colliderAngle),
		point.x * std::sin(colliderAngle) + point.y * std::cos(colliderAngle));

	sf::Vector2f global = point + m_Offset;

	global.x *= scale.x;
	global.y *= scale.y;

	global = sf::Vector2f(
		global.x * std::cos(angle) - global.y * std::sin(angle),
		global.x * std::sin(angle) + global.y * std::cos(angle));

	global += trans.getPosition();

	return global;
}

Basic::BoxShape::BoxShape()
	: m_SFMLRect(sf::RectangleShape()), m_Offset(sf::Vector2f(0.0f, 0.0f)), m_Rotation(0.0f)
{ }

Basic::BoxShape::BoxShape(sf::Vector2f size)
	: m_SFMLRect(sf::RectangleShape(size)), m_Offset(sf::Vector2f(0.0f, 0.0f)), m_Rotation(0.0f)
{ 
	m_SFMLRect.setOrigin(size.x / 2.0f, size.y / 2.0f);

	m_Vertices[0] = sf::Vector2f(-size.x / 2.0f, -size.y / 2.0f);
	m_Vertices[1] = sf::Vector2f(size.x / 2.0f, -size.y / 2.0f);
	m_Vertices[2] = sf::Vector2f(size.x / 2.0f, size.y / 2.0f);
	m_Vertices[3] = sf::Vector2f(-size.x / 2.0f, size.y / 2.0f);
}

void Basic::BoxShape::SetFillColor(const sf::Color& color)
{
	m_SFMLRect.setFillColor(color);
}

sf::Color Basic::BoxShape::GetFillColor() const
{
	return m_SFMLRect.getFillColor();
}

void Basic::BoxShape::SetOutlineColor(const sf::Color& color)
{
	m_SFMLRect.setOutlineColor(color);
}

sf::Color Basic::BoxShape::GetOutlineColor() const
{
	return m_SFMLRect.getOutlineColor();
}

void Basic::BoxShape::SetOffset(sf::Vector2f offset)
{
	m_Offset = offset;
	m_SFMLRect.setPosition(m_Offset);
}

sf::Vector2f Basic::BoxShape::GetOffset() const
{
	return m_Offset;
}

void Basic::BoxShape::SetRotation(float rotation)
{
	m_Rotation = rotation;
	m_SFMLRect.setRotation(m_Rotation);
}

float Basic::BoxShape::GetRotation() const
{
	return m_Rotation;
}

void Basic::BoxShape::SetOutlineThickness(float thickness)
{
	m_SFMLRect.setOutlineThickness(thickness);
}

float Basic::BoxShape::GetOutlineThickness() const
{
	return m_SFMLRect.getOutlineThickness();
}

Basic::AABB Basic::BoxShape::GetGlobalAABB(const Transform& transform) const
{
	Basic::AABB globalAABB;
	sf::Vector2f v1 = TranslateRelativePointToGlobal(m_Vertices[0], transform);
	sf::Vector2f v2 = TranslateRelativePointToGlobal(m_Vertices[1], transform);
	sf::Vector2f v3 = TranslateRelativePointToGlobal(m_Vertices[2], transform);
	sf::Vector2f v4 = TranslateRelativePointToGlobal(m_Vertices[3], transform);

	globalAABB.minPoint = sf::Vector2f(std::min(std::min(v1.x, v2.x), std::min(v3.x, v4.x)),
		std::min(std::min(v1.y, v2.y), std::min(v3.y, v4.y)));

	globalAABB.maxPoint = sf::Vector2f(std::max(std::max(v1.x, v2.x), std::max(v3.x, v4.x)), 
		std::max(std::max(v1.y, v2.y), std::max(v3.y, v4.y)));

	return globalAABB;
}

sf::Vector2f Basic::BoxShape::GetGlobalPosition(const Transform& transform) const
{
	return TranslateRelativePointToGlobal(sf::Vector2f(0.0f, 0.0f), transform);
}
