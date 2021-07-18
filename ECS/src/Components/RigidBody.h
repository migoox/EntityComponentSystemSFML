#pragma once
#include <SFML/Graphics.hpp>

namespace Basic {
	struct RigidBody
	{
		sf::Vector2f Velocity;
		sf::Vector2f Acceleration;
		sf::Vector2f Force;
	};
}