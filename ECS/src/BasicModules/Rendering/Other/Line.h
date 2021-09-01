#pragma once
#include <SFML/Graphics.hpp>

namespace Basic {
    class Line : public sf::Drawable, public sf::Transformable
    {
    private:
        sf::Vector2f m_PointA;
        sf::Vector2f m_PointB;

        float m_Thickness;

        sf::Color m_Color;

        sf::VertexArray m_VertexArray;

        const sf::Texture* m_Texture;

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        void UpdatePoints();

        void UpdateColor();

    public:
        Line();

        Line(sf::Vector2f pointA, sf::Vector2f pointB, float thickness, sf::Color color);
        Line(sf::Vector2f pointA, float distance, float thickness, sf::Color color);
        Line(float distance, float thickness, sf::Color color);

        void setPoints(sf::Vector2f pointA, sf::Vector2f pointB);

        void setColor(sf::Color color);

        void setThickness(float thickness);

        void setRotation(float angle) = delete;
        void setScale(float factorX, float factorY) = delete;
        void setScale(const sf::Vector2f& factors) = delete;
    };
}