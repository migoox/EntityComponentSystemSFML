#include "RectangleCollider.h"

void Basic::RectangleCollider::Init()
{
	m_Vertices.resize(4);
	m_GlobalVertices.resize(4);
	if (m_Size.x == 0.0f || m_Size.y == 0.0f)
	{
		for (size_t i = 0; i < 4; i++)
		{
			m_Vertices[i] = sf::Vector2f(0.0f, 0.0f);
		}
	}
	else
	{
		m_Vertices[0] = sf::Vector2f(-m_Size.x / 2.0f, -m_Size.y / 2.0f);
		m_Vertices[1] = sf::Vector2f(m_Size.x / 2.0f, -m_Size.y / 2.0f);
		m_Vertices[2] = sf::Vector2f(m_Size.x / 2.0f, m_Size.y / 2.0f);
		m_Vertices[3] = sf::Vector2f(-m_Size.x / 2.0f, m_Size.y / 2.0f);
	}
}

sf::Vector2f Basic::RectangleCollider::TranslateRelativePointToGlobal(sf::Vector2f point, const Transform& trans) const
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

void Basic::RectangleCollider::UpdateGlobalVertices(const Transform& trans) const
{
	m_GlobalVertices[0] = TranslateRelativePointToGlobal(m_Vertices[0], trans);
	m_GlobalVertices[1] = TranslateRelativePointToGlobal(m_Vertices[1], trans);
	m_GlobalVertices[2] = TranslateRelativePointToGlobal(m_Vertices[2], trans);
	m_GlobalVertices[3] = TranslateRelativePointToGlobal(m_Vertices[3], trans);
}

void Basic::RectangleCollider::MoveCollider(sf::Vector2f displacement)
{
	m_ColliderDisplacement += displacement;
}

void Basic::RectangleCollider::RotateCollider(float angle)
{
	m_ColliderRotation += angle;
}

sf::Vector2f Basic::RectangleCollider::GetGlobalCenterOfGravity(const Transform& trans) const
{
	return TranslateRelativePointToGlobal(sf::Vector2f(0.0f, 0.0f), trans);
}

float Basic::RectangleCollider::GetMomentOfInertia(const RigidBody& rb) const
{
	float A = 0.0f;
	float B = 0.0f;

	for (size_t i = 0; i < m_Vertices.size() - 1; i++)
		A += m_Vertices[i].x * m_Vertices[i + 1].y - m_Vertices[i + 1].x * m_Vertices[i].y;

	for (size_t i = 0; i < m_Vertices.size() - 1; i++)
	{
		auto& i1 = m_Vertices[i];
		auto& i2 = m_Vertices[i + 1];

		B += (std::pow(i1.x, 2) + std::pow(i1.y, 2) + i1.x * i2.x + i1.y * i2.y + std::pow(i2.y, 2) + std::pow(i2.y, 2)) * (i1.x * i2.y - i2.x * i1.y);
	}

	return B / A / 6 * rb.Mass;
}

void Basic::RectangleCollider::DrawOnceOnVisualGizmos(const Transform& trans) const
{
	UpdateGlobalVertices(trans);
	VertexArray arr(sf::PrimitiveType::Quads, 4);
	arr[0] = m_GlobalVertices[0];
	arr[1] = m_GlobalVertices[1];
	arr[2] = m_GlobalVertices[2];
	arr[3] = m_GlobalVertices[3];

	arr[0].color = sf::Color::Blue;
	arr[1].color = sf::Color::Blue;
	arr[2].color = sf::Color::Blue;
	arr[3].color = sf::Color::Blue;

	CircleShape gCenter(2.0f);
	gCenter.setFillColor(sf::Color::Red);
	gCenter.setOrigin(1.0f, 1.0f);
	gCenter.setPosition(GetGlobalCenterOfGravity(trans));

	Basic::VisualGizmos::DrawOnce(arr);
	Basic::VisualGizmos::DrawOnce(gCenter);
}

const std::vector<sf::Vector2f>& Basic::RectangleCollider::GlobalVertices(const Transform& trans) const
{
	UpdateGlobalVertices(trans);

	return m_GlobalVertices;
}
