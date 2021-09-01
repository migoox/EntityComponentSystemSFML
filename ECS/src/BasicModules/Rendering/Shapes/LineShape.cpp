#include "LineShape.h"

sf::Vector2f Basic::LineShape::TranslateRelativePointToGlobal(sf::Vector2f point, const Transform& trans) const
{
	sf::Vector2f scale = trans.getScale();
	float angle = trans.getRotation() * 3.141592653f / 180.0f;

	sf::Vector2f global = point + m_Offset;

	global.x *= scale.x;
	global.y *= scale.y;

	global = sf::Vector2f(
		global.x * std::cos(angle) - global.y * std::sin(angle),
		global.x * std::sin(angle) + global.y * std::cos(angle));

	global += trans.getPosition();

	return global;
}

void Basic::LineShape::UpdateVertexArray()
{
	if (m_LocalPointA == m_LocalPointB)
	{
		m_VertexArray[0].position = m_LocalPointA + m_Offset;
		m_VertexArray[1].position = m_LocalPointA + m_Offset;
		m_VertexArray[2].position = m_LocalPointA + m_Offset;
		m_VertexArray[3].position = m_LocalPointA + m_Offset;

		return;
	}

	sf::Vector2f ABVector = m_LocalPointB - m_LocalPointA;
	sf::Vector2f ABVectorNormalized = ABVector / static_cast<float>((sqrt(pow(ABVector.x, 2) + pow(ABVector.y, 2))));

	sf::Vector2f offsetVectorRight = m_Thickness / 2.f * sf::Vector2f(ABVectorNormalized.y, -ABVectorNormalized.x);
	sf::Vector2f offsetVectorLeft = -offsetVectorRight;

	m_VertexArray[0].position = m_LocalPointB + offsetVectorRight + m_Offset;
	m_VertexArray[1].position = m_LocalPointA + offsetVectorRight + m_Offset;
	m_VertexArray[2].position = m_LocalPointA + offsetVectorLeft + m_Offset;
	m_VertexArray[3].position = m_LocalPointB + offsetVectorLeft + m_Offset;
}

void Basic::LineShape::UpdateColors()
{
	m_VertexArray[0].color = m_Color;
	m_VertexArray[1].color = m_Color;
	m_VertexArray[2].color = m_Color;
	m_VertexArray[3].color = m_Color;
}

Basic::LineShape::LineShape()
	: m_LocalPointA(sf::Vector2f(0.0f, 0.0f)), m_LocalPointB(sf::Vector2f(0.0f, 0.0f)),
	m_Thickness(0.0f), m_Color(sf::Color::White), m_Offset(sf::Vector2f(0.0f, 0.0f)), m_Rotation(0.0f)
{
	m_VertexArray.resize(4);
	m_VertexArray.setPrimitiveType(sf::PrimitiveType::Quads);
}

Basic::LineShape::LineShape(float distance, float thickness)
	: m_LocalPointA(sf::Vector2f(0.0f, 0.0f)), m_LocalPointB(sf::Vector2f(0.0f, 0.0f)),
	m_Thickness(thickness), m_Color(sf::Color::White), m_Offset(sf::Vector2f(0.0f, 0.0f)), m_Rotation(0.0f)
{ 
	m_VertexArray.resize(4);
	m_VertexArray.setPrimitiveType(sf::PrimitiveType::Quads);

	m_LocalPointA = sf::Vector2f(-distance / 2.0f, 0.0f);
	m_LocalPointB = sf::Vector2f(distance / 2.0f, 0.0f);

	UpdateVertexArray();
}

void Basic::LineShape::SetFillColor(const sf::Color& color)
{
	m_Color = color;
	UpdateColors();
}

sf::Color Basic::LineShape::GetFillColor() const
{
	return m_Color;
}

void Basic::LineShape::SetOffset(sf::Vector2f offset)
{
	m_Offset = offset;
	UpdateVertexArray();
}

sf::Vector2f Basic::LineShape::GetOffset() const
{
	return m_Offset;
}

void Basic::LineShape::SetRotation(float rotation)
{
	m_Rotation = rotation;

	float colliderAngle = m_Rotation * 3.141592653f / 180.0f;

	m_LocalPointA = sf::Vector2f(
		m_LocalPointA.x * std::cos(colliderAngle) - m_LocalPointA.y * std::sin(colliderAngle),
		m_LocalPointA.x * std::sin(colliderAngle) + m_LocalPointA.y * std::cos(colliderAngle));

	m_LocalPointB = sf::Vector2f(
		m_LocalPointB.x * std::cos(colliderAngle) - m_LocalPointB.y * std::sin(colliderAngle),
		m_LocalPointB.x * std::sin(colliderAngle) + m_LocalPointB.y * std::cos(colliderAngle));

	UpdateVertexArray();
}

float Basic::LineShape::GetRotation() const
{
	return m_Rotation;
}

Basic::AABB Basic::LineShape::GetGlobalAABB(const Transform& transform) const
{
	Basic::AABB aabb;
	sf::Vector2f AGlobal = TranslateRelativePointToGlobal(m_LocalPointA, transform);
	sf::Vector2f BGlobal = TranslateRelativePointToGlobal(m_LocalPointB, transform);

	aabb.minPoint = sf::Vector2f(std::min(AGlobal.x, BGlobal.x), std::min(AGlobal.y, BGlobal.y));
	aabb.maxPoint = sf::Vector2f(std::max(AGlobal.x, BGlobal.x), std::max(AGlobal.y, BGlobal.y));

	return aabb;
}

sf::Vector2f Basic::LineShape::GetGlobalPosition(const Transform& transform) const
{
	return TranslateRelativePointToGlobal(sf::Vector2f(0.0f, 0.0f), transform);
}