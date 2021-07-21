#pragma once
#include <SFML/Graphics.hpp>

namespace Basic {
	class ConvexShape : public sf::ConvexShape
	{
	public:
		using sf::ConvexShape::ConvexShape;

		void setRotation(float angle) = delete;
		void setScale(float factorX, float factorY) = delete;
		void setScale(const sf::Vector2f& factors) = delete;
	};
}