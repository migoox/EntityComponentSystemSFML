#pragma once
#include <math.h>
#include <SFML/Graphics.hpp>

namespace Basic {
	namespace MathFunctions {
		float DistanceFromLine(sf::Vector2f point, sf::Vector2f lineA, sf::Vector2f lineB);
		float Distance(sf::Vector2f pointA, sf::Vector2f pointB);
		float VectorDistance(sf::Vector2f vector);
		sf::Vector2f NormalizeVector(sf::Vector2f vector);
		float IsUnderLine(sf::Vector2f point, sf::Vector2f lineA, sf::Vector2f lineB);
		float DotOfVectors(sf::Vector2f vector1, sf::Vector2f vector2);

	}
}