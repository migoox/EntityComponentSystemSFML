#include "Maths.h"

float Basic::MathFunctions::DistanceFromLine(sf::Vector2f point, sf::Vector2f lineA, sf::Vector2f lineB)
{
	// counting general linear equation which contain point A and B, p stands for "parameter"
	float Ap = lineA.y - lineB.y;
	float Bp = lineB.x - lineA.x;
	float Cp = lineA.x * (lineB.y - lineA.y) - lineA.y * (lineB.x - lineA.x);

	// counting distance of circle from the line
	float circleplaneDistance = std::abs(point.x * Ap + point.y * Bp + Cp) / sqrt(pow(Ap, 2) + pow(Bp, 2));

	return circleplaneDistance;
}

float Basic::MathFunctions::Distance(sf::Vector2f pointA, sf::Vector2f pointB)
{
	return sqrt(pow(pointA.x - pointB.x, 2) +
		pow(pointA.y - pointB.y, 2));
}

float Basic::MathFunctions::VectorDistance(sf::Vector2f vector)
{
	return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

sf::Vector2f Basic::MathFunctions::NormalizeVector(sf::Vector2f vector)
{
	return vector / MathFunctions::VectorDistance(vector);
}

float Basic::MathFunctions::IsUnderLine(sf::Vector2f point, sf::Vector2f lineA, sf::Vector2f lineB)
{
	// counting general linear equations which contain point A and B, p stands for "parameter"
	float Ap = lineA.y - lineB.y;
	float Bp = lineB.x - lineA.x;
	float Cp = lineA.x * (lineB.y - lineA.y) - lineA.y * (lineB.x - lineA.x);

	if (Bp == 0.0f)
		return false;

	// counting y on line with given x
	float x = point.x;
	float y = -(Ap * point.x + Cp) / Bp;

	if (y < point.y)
		return false;

	return true;
}

float Basic::MathFunctions::DotOfVectors(sf::Vector2f vector1, sf::Vector2f vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y;
}