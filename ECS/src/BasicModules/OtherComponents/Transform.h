#pragma once
#include <SFML/Graphics.hpp>

namespace Basic {
	struct Transform : public sf::Transformable
	{
		void setOrigin(float x, float y) = delete;
		void setOrigin(const sf::Vector2f& origin) = delete;

		const sf::Vector2f& getOrigin() = delete;
	};
}