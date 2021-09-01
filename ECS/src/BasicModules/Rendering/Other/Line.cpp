#include "Line.h"

void Basic::Line::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the entity's transform -- combine it with the one that was passed by the caller
    states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

    // apply the texture
    // states.texture = m_Texture;

    // you may also override states.shader or states.blendMode if you want

    // draw the vertex array
    target.draw(m_VertexArray, states);
}

void Basic::Line::UpdatePoints()
{
    if (m_PointA == m_PointB)
    {
        m_VertexArray[0].position = m_PointA;
        m_VertexArray[1].position = m_PointA;
        m_VertexArray[2].position = m_PointA;
        m_VertexArray[3].position = m_PointA;

        return;
    }

    sf::Vector2f ABVector = m_PointB - m_PointA;
    sf::Vector2f ABVectorNormalized = ABVector / static_cast<float>((sqrt(pow(ABVector.x, 2) + pow(ABVector.y, 2))));

    sf::Vector2f offsetVectorRight = m_Thickness / 2.f * sf::Vector2f(ABVectorNormalized.y, -ABVectorNormalized.x);
    sf::Vector2f offsetVectorLeft = -offsetVectorRight;

    m_VertexArray[0].position = m_PointB + offsetVectorRight;
    m_VertexArray[1].position = m_PointA + offsetVectorRight;
    m_VertexArray[2].position = m_PointA + offsetVectorLeft;
    m_VertexArray[3].position = m_PointB + offsetVectorLeft;
}

void Basic::Line::UpdateColor()
{
    m_VertexArray[0].color = m_Color;
    m_VertexArray[1].color = m_Color;
    m_VertexArray[2].color = m_Color;
    m_VertexArray[3].color = m_Color;
}

Basic::Line::Line()
{
    m_VertexArray.setPrimitiveType(sf::Quads);
    m_VertexArray.resize(4);
}

Basic::Line::Line(sf::Vector2f pointA, sf::Vector2f pointB, float thickness, sf::Color color)
    : m_PointA(pointA), m_PointB(pointB), m_Thickness(thickness), m_Color(color)

{
    m_VertexArray.setPrimitiveType(sf::Quads);
    m_VertexArray.resize(4);

    UpdatePoints();

    UpdateColor();
}

Basic::Line::Line(sf::Vector2f pointA, float distance, float thickness, sf::Color color)
    : m_PointA(pointA), m_PointB(sf::Vector2f(pointA.x + distance, pointA.y)), m_Thickness(thickness), m_Color(color)
{
    m_VertexArray.setPrimitiveType(sf::Quads);
    m_VertexArray.resize(4);

    UpdatePoints();

    UpdateColor();
}

Basic::Line::Line(float distance, float thickness, sf::Color color)
    : m_PointA(sf::Vector2f(0.0f, 0.0f)), m_PointB(sf::Vector2f(distance, 0.0f)), m_Thickness(thickness), m_Color(color)
{
    m_VertexArray.setPrimitiveType(sf::Quads);
    m_VertexArray.resize(4);

    UpdatePoints();

    UpdateColor();
}

void Basic::Line::setPoints(sf::Vector2f pointA, sf::Vector2f pointB)
{
    m_PointA = pointA;

    m_PointB = pointB;

    UpdatePoints();
}

void Basic::Line::setColor(sf::Color color)
{
    m_Color = color;

    UpdateColor();
}

void Basic::Line::setThickness(float thickness)
{
    m_Thickness = thickness;

    UpdatePoints();
}

