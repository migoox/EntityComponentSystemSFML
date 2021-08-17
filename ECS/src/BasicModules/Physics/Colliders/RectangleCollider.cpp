#include "RectangleCollider.h"

Basic::RectangleCollider::RectangleCollider()
{
	m_Vertices.resize(4);
	m_GlobalVertices.resize(4);

	m_Correct = false;
	m_Convex = true;

	m_Fixer = sf::Vector2f(0.0f, 0.0f);

	for (size_t i = 0; i < 4; i++)
	{
		m_Vertices[i] = sf::Vector2f(0.0f, 0.0f);
	}
}

Basic::RectangleCollider::RectangleCollider(sf::Vector2f size)
	: m_Size(size)
{
	m_Vertices.resize(4);
	m_GlobalVertices.resize(4);

	m_Correct = true;
	m_Convex = true;

	m_Fixer = sf::Vector2f(0.0f, 0.0f);

	m_Vertices[0] = sf::Vector2f(-m_Size.x / 2.0f, -m_Size.y / 2.0f);
	m_Vertices[1] = sf::Vector2f(m_Size.x / 2.0f, -m_Size.y / 2.0f);
	m_Vertices[2] = sf::Vector2f(m_Size.x / 2.0f, m_Size.y / 2.0f);
	m_Vertices[3] = sf::Vector2f(-m_Size.x / 2.0f, m_Size.y / 2.0f);
}

Basic::RectangleCollider::RectangleCollider(sf::Vector2f size, sf::Vector2f displacement)
	: m_Size(size)
{
	m_Vertices.resize(4);
	m_GlobalVertices.resize(4);

	m_ColliderDisplacement = displacement;

	m_Correct = true;
	m_Convex = true;

	m_Fixer = sf::Vector2f(0.0f, 0.0f);

	m_Vertices[0] = sf::Vector2f(-m_Size.x / 2.0f, -m_Size.y / 2.0f);
	m_Vertices[1] = sf::Vector2f(m_Size.x / 2.0f, -m_Size.y / 2.0f);
	m_Vertices[2] = sf::Vector2f(m_Size.x / 2.0f, m_Size.y / 2.0f);
	m_Vertices[3] = sf::Vector2f(-m_Size.x / 2.0f, m_Size.y / 2.0f);
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

void Basic::RectangleCollider::Set(sf::Vector2f size)
{
	m_Vertices.resize(4);
	m_GlobalVertices.resize(4);

	m_Correct = true;
	m_Convex = true;

	m_Fixer = sf::Vector2f(0.0f, 0.0f);

	m_Vertices[0] = sf::Vector2f(-m_Size.x / 2.0f, -m_Size.y / 2.0f);
	m_Vertices[1] = sf::Vector2f(m_Size.x / 2.0f, -m_Size.y / 2.0f);
	m_Vertices[2] = sf::Vector2f(m_Size.x / 2.0f, m_Size.y / 2.0f);
	m_Vertices[3] = sf::Vector2f(-m_Size.x / 2.0f, m_Size.y / 2.0f);
}

void Basic::RectangleCollider::Set(sf::Vector2f size, sf::Vector2f displacement)
{
	m_Vertices.resize(4);
	m_GlobalVertices.resize(4);

	m_ColliderDisplacement = displacement;

	m_Correct = true;
	m_Convex = true;

	m_Fixer = sf::Vector2f(0.0f, 0.0f);

	m_Vertices[0] = sf::Vector2f(-m_Size.x / 2.0f, -m_Size.y / 2.0f);
	m_Vertices[1] = sf::Vector2f(m_Size.x / 2.0f, -m_Size.y / 2.0f);
	m_Vertices[2] = sf::Vector2f(m_Size.x / 2.0f, m_Size.y / 2.0f);
	m_Vertices[3] = sf::Vector2f(-m_Size.x / 2.0f, m_Size.y / 2.0f);
}

