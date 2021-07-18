#pragma once
#include <SFML/Graphics.hpp>

namespace Basic {
	class CircleShape : public sf::CircleShape
	{
	public:
		using sf::CircleShape::CircleShape;

		void setRotation(float angle) = delete;
		void setScale(float factorX, float factorY) = delete;
		void setScale(const sf::Vector2f& factors) = delete;
	};
}