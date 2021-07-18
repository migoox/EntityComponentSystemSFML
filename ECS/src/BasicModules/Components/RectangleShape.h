#pragma once
#include <SFML/Graphics.hpp>

namespace Basic {
	class RectangleShape : public sf::RectangleShape
	{
	public:
		using sf::RectangleShape::RectangleShape;

		void setRotation(float angle) = delete;
		void setScale(float factorX, float factorY) = delete;
		void setScale(const sf::Vector2f& factors) = delete;
	};
}