#pragma once
#include <math.h>
#include <SFML/Graphics.hpp>

namespace Basic {
	namespace Maths {
		float DistanceFromLine(sf::Vector2f point, sf::Vector2f lineA, sf::Vector2f lineB);
		float Distance(sf::Vector2f pointA, sf::Vector2f pointB);
		float VectorDistance(sf::Vector2f vector);
		sf::Vector2f NormalizeVector(sf::Vector2f vector);
		float IsUnderLine(sf::Vector2f point, sf::Vector2f lineA, sf::Vector2f lineB);

		float Dot(sf::Vector2f vecA, sf::Vector2f vecB);
		float Dot(sf::Vector3f vecA, sf::Vector3f vecB);

		sf::Vector3f Cross(sf::Vector3f vecA, sf::Vector3f vecB);
		sf::Vector2f Cross(sf::Vector2f vecA, float vecB);
		float Cross(sf::Vector2f vecA, sf::Vector2f vecB);

		sf::Vector2f TripleProduct(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c);
	}
}