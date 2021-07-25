#include "CollisionDetectionAlgorithms.h"
#include "../PhysicalComponents/CircleCollider.h"
#include "../PhysicalComponents/PlaneCollider.h"
#include "../Game.h"

#include <iostream>

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
	// counting general linear equation which contain point A and B, p stands for "parameter"
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

Basic::CollisionPoints Basic::CollisionDetection::FindCircleCircleCollisionPoints(
	const CircleCollider& circle1, const Transform& transform1, 
	const CircleCollider& circle2, const Transform& transform2)
{
	sf::Vector2f globalPosition1 = circle1.Center + transform1.getPosition();
	sf::Vector2f globalPosition2 = circle2.Center + transform2.getPosition();

	float distanceBetweenCenters = sqrt(pow(globalPosition1.x - globalPosition2.x, 2) +
		pow(globalPosition1.y - globalPosition2.y, 2));

	CollisionPoints collPoints; // default parameters
	collPoints.HasCollision = false;

	if (distanceBetweenCenters < circle1.Radius + circle2.Radius)
	{
		// collision
		collPoints.HasCollision = true;
		collPoints.Depth = circle1.Radius + circle2.Radius - distanceBetweenCenters;
		collPoints.Normal = (globalPosition2 - globalPosition1) / distanceBetweenCenters;
		collPoints.A = globalPosition1 + circle1.Radius * collPoints.Normal;
		collPoints.B = globalPosition2 + circle2.Radius * (-collPoints.Normal);
	}

	return collPoints;
}

Basic::CollisionPoints Basic::CollisionDetection::FindCirclePlaneCollisionPoints(
	const CircleCollider& circle, const Transform& circleTransform, 
	const PlaneCollider& plane, const Transform& planeTransform)
{
	float pi = 3.141592653f;

	// counting points
	sf::Vector2f pointA;
	sf::Vector2f pointB;
	sf::Vector2f center;

	pointA = plane.Plane + planeTransform.getPosition();

	pointB.x = plane.Plane.x + std::cos(planeTransform.getRotation() * pi / 180.0f) * plane.Distance + planeTransform.getPosition().x;
	pointB.y = plane.Plane.y + std::sin(planeTransform.getRotation() * pi / 180.0f) * plane.Distance + planeTransform.getPosition().y;

	center = circle.Center + circleTransform.getPosition();

	// counting vectors
	sf::Vector2f CenterPlaneVector = sf::Vector2f(pointB.y - pointA.y, -(pointB.x - pointA.x)) 
		/ plane.Distance * MathFunctions::DistanceFromLine(center, pointA, pointB);

	if (MathFunctions::IsUnderLine(center, pointA, pointB))
		CenterPlaneVector = (-1.0f) * CenterPlaneVector;

	sf::Vector2f PointACenterVector = sf::Vector2f(center.x - pointA.x, center.y - pointA.y);

	sf::Vector2f ShadowVector = CenterPlaneVector + PointACenterVector;

	sf::Vector2f ShadowVectorNormalized = MathFunctions::NormalizeVector(ShadowVector);
	sf::Vector2f ABNormalized = MathFunctions::NormalizeVector(sf::Vector2f(pointB.x - pointA.x, pointB.y - pointA.y));
	//sf::Vector2f CenterToShadowCenterVector = MathFunctions::DistanceFromLine(center, pointA, pointB);


	sf::VertexArray arr1(sf::Lines, 2);
	arr1[0].position = pointA;
	arr1[1].position = pointA + PointACenterVector;
	arr1[0].color = sf::Color::Red;
	arr1[1].color = sf::Color::Green;

	sf::VertexArray arr2(sf::Lines, 2);
	arr2[0].position = center;
	arr2[1].position = center + CenterPlaneVector;
	arr2[0].color = sf::Color::Red;
	arr2[1].color = sf::Color::Green;

	sf::VertexArray arr3(sf::Lines, 2);
	arr3[0].position = pointA;
	arr3[1].position = pointA + ShadowVector;
	arr3[0].color = sf::Color::Red;
	arr3[1].color = sf::Color::Green;

	Game::TempDraw1(arr1);
	Game::TempDraw2(arr2);
	Game::TempDraw3(arr3);




	CollisionPoints collPoints; // default parameters
	collPoints.HasCollision = false;

	if (MathFunctions::VectorDistance(CenterPlaneVector) < circle.Radius)
	{
		if (MathFunctions::VectorDistance(ShadowVector) < plane.Distance)
		{
			if (true)
			{
				collPoints.HasCollision = true;
			}
		}
	}

	return collPoints;
}

Basic::CollisionPoints Basic::CollisionDetection::FindPlaneCircleCollisionPoints(
	const PlaneCollider& plane, const Transform& planeTransform, 
	const CircleCollider& circle, const Transform& circleTransform)
{
	return CollisionPoints();
}
