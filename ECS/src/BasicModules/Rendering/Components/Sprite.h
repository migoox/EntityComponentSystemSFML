#pragma once
#include <SFML/Graphics.hpp>

namespace Basic {
	class Sprite : public sf::Sprite
	{
	public:
		using sf::Sprite::Sprite;

		void setScale(float factorX, float factorY) = delete;
		void setScale(const sf::Vector2f& factors) = delete;
	};
}