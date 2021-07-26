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

Basic::CollisionPoints Basic::CollisionDetection::FindCircleCircleCollisionPoints(
	const CircleCollider* circle1, const Transform& transform1, 
	const CircleCollider* circle2, const Transform& transform2)
{
	sf::Vector2f globalPosition1 = circle1->GetGlobalCenter(transform1);
	sf::Vector2f globalPosition2 = circle2->GetGlobalCenter(transform2);

	float distanceBetweenCenters = sqrt(pow(globalPosition1.x - globalPosition2.x, 2) +
		pow(globalPosition1.y - globalPosition2.y, 2));

	CollisionPoints collPoints; // default parameters
	collPoints.HasCollision = false;

	if (distanceBetweenCenters < circle1->Radius + circle2->Radius)
	{
		// collision
		collPoints.HasCollision = true;
		collPoints.Depth = circle1->Radius + circle2->Radius - distanceBetweenCenters;
		collPoints.Normal = (globalPosition2 - globalPosition1) / distanceBetweenCenters;
		collPoints.A = globalPosition1 + circle1->Radius * collPoints.Normal;
		collPoints.B = globalPosition2 + circle2->Radius * (-collPoints.Normal);
	}

	return collPoints;
}

Basic::CollisionPoints Basic::CollisionDetection::FindCirclePlaneCollisionPoints(
	const CircleCollider* circle, const Transform& circleTransform, 
	const PlaneCollider* plane, const Transform& planeTransform)
{
	float pi = 3.141592653f;

	// counting points
	sf::Vector2f pointA = plane->GetGlobalAPoint(planeTransform);
	sf::Vector2f pointB = plane->GetGlobalBPoint(planeTransform);
	sf::Vector2f center = circle->GetGlobalCenter(circleTransform);

	// counting vectors
	sf::Vector2f CenterPlaneVector = sf::Vector2f(pointB.y - pointA.y, -(pointB.x - pointA.x)) 
		/ plane->Distance * MathFunctions::DistanceFromLine(center, pointA, pointB);

	if (MathFunctions::IsUnderLine(center, pointA, pointB))
		CenterPlaneVector = (-1.0f) * CenterPlaneVector;

	sf::Vector2f PointACenterVector = sf::Vector2f(center.x - pointA.x, center.y - pointA.y);

	sf::Vector2f ShadowVector = CenterPlaneVector + PointACenterVector;

	sf::Vector2f ShadowVectorNormalized = MathFunctions::NormalizeVector(ShadowVector);

	// create collision points
	CollisionPoints collPoints;

	if (MathFunctions::Distance(center, pointA) <= circle->Radius ||
		MathFunctions::Distance(center, pointB) <= circle->Radius) // one of the points of edge is inside the circle
	{
		collPoints.HasCollision = true;
	}
	else
	{
		if (MathFunctions::VectorDistance(CenterPlaneVector) <= circle->Radius)
		{
			if (MathFunctions::VectorDistance(ShadowVector) <= plane->Distance)
			{
				sf::Vector2f ABVector = sf::Vector2f(pointB.x - pointA.x, pointB.y - pointA.y);

				if (ABVector.x * ShadowVector.x + ABVector.y * ShadowVector.y >= 0)
				{
					collPoints.HasCollision = true;
					collPoints.Depth = plane->Distance;
					collPoints.A = center + CenterPlaneVector;
					collPoints.B = center + CenterPlaneVector / MathFunctions::VectorDistance(CenterPlaneVector) * circle->Radius;
					collPoints.Normal = sf::Vector2f(collPoints.A.x - collPoints.B.x, 
						collPoints.A.y - collPoints.B.y) / collPoints.Depth;

					sf::VertexArray arr(sf::Points, 2);
					arr[0].position = collPoints.A;
					arr[1].position = collPoints.B;
					arr[0].color = sf::Color::Red;
						arr[1].color = sf::Color::Red;
					Game::TempDraw1(arr);
				}
			}
		}
	}
	return collPoints;
}

Basic::CollisionPoints Basic::CollisionDetection::FindPlanePlaneCollisionPoints(
	const PlaneCollider* plane1, const Transform& plane1Transform, 
	const PlaneCollider* plane2, const Transform& plane2Transform)
{
	float pi = 3.141592653f;

	// counting points
	sf::Vector2f pointA1 = plane1->GetGlobalAPoint(plane1Transform);
	sf::Vector2f pointB1 = plane1->GetGlobalBPoint(plane1Transform);
	sf::Vector2f pointA2 = plane2->GetGlobalAPoint(plane2Transform);
	sf::Vector2f pointB2 = plane2->GetGlobalBPoint(plane2Transform);

	// creating collision points
	CollisionPoints collPoints;
	collPoints.Resolvable = false; // since that, it is not necessary to count collision points

	if (pointA1.x == pointB1.x && pointA2.x == pointB2.x) // no intersection (both are vertical)
	{
		collPoints.HasCollision = false;
	}
	else
	{
		if (pointA1.x == pointB1.x) // first one isn't a function
		{
			// counting parameters for second line
			float a2 = (pointB2.y - pointA2.y) / (pointB2.x - pointA2.x);
			float b2 = pointA2.y - a2 * pointA2.x;

			// counting intersection point
			float intersectionX = pointA1.x;

			sf::Vector2f intersectionPoint = sf::Vector2f(intersectionX, intersectionX * a2 + b2);

			// collision condition
			if (intersectionPoint.y >= pointA1.y && intersectionPoint.y <= pointB1.y ||
				intersectionPoint.y <= pointA1.y && intersectionPoint.y >= pointB1.y)
			{
				if (intersectionPoint.x >= pointA2.x && intersectionPoint.x <= pointB2.x ||
					intersectionPoint.x <= pointA2.x && intersectionPoint.x >= pointB2.x)
				{
					if (intersectionPoint.y >= pointA2.y && intersectionPoint.y <= pointB2.y ||
						intersectionPoint.y <= pointA2.y && intersectionPoint.y >= pointB2.y)
					{
						collPoints.HasCollision = true;
					}
				}
			}
		}
		else if (pointA2.x == pointB2.x) // second one isn't a function
		{
			// counting parameters for second line
			float a1 = (pointB1.y - pointA1.y) / (pointB1.x - pointA1.x);
			float b1 = pointA1.y - a1 * pointA1.x;

			// counting intersection point
			float intersectionX = pointA2.x;

			sf::Vector2f intersectionPoint = sf::Vector2f(intersectionX, intersectionX * a1 + b1);

			// collision condition
			if (intersectionPoint.y >= pointA2.y && intersectionPoint.y <= pointB2.y ||
				intersectionPoint.y <= pointA2.y && intersectionPoint.y >= pointB2.y)
			{
				if (intersectionPoint.x >= pointA1.x && intersectionPoint.x <= pointB1.x ||
					intersectionPoint.x <= pointA1.x && intersectionPoint.x >= pointB1.x)
				{
					if (intersectionPoint.y >= pointA1.y && intersectionPoint.y <= pointB1.y ||
						intersectionPoint.y <= pointA1.y && intersectionPoint.y >= pointB1.y)
					{
						collPoints.HasCollision = true;
					}
				}
			}
		}
		else
		{
			// counting direction parameters
			float a1 = (pointB1.y - pointA1.y) / (pointB1.x - pointA1.x);
			float a2 = (pointB2.y - pointA2.y) / (pointB2.x - pointA2.x);

			if (a1 == a2) // no intersection (they are in parallel to each other) 
			{
				collPoints.HasCollision = false;
			}
			else // line functions are intersecting with each other
			{
				float b1 = pointA1.y - a1 * pointA1.x;
				float b2 = pointA2.y - a2 * pointA2.x;

				// counting intersection point
				float intersectionX = (b1 - b2) / (a2 - a1);
				sf::Vector2f intersectionPoint = sf::Vector2f(intersectionX, intersectionX * a1 + b1);

				// collision condition
				if (intersectionPoint.x >= pointA2.x && intersectionPoint.x <= pointB2.x ||
					intersectionPoint.x <= pointA2.x && intersectionPoint.x >= pointB2.x)
				{
					if (intersectionPoint.y >= pointA2.y && intersectionPoint.y <= pointB2.y ||
						intersectionPoint.y <= pointA2.y && intersectionPoint.y >= pointB2.y)
					{
						if (intersectionPoint.x >= pointA1.x && intersectionPoint.x <= pointB1.x ||
							intersectionPoint.x <= pointA1.x && intersectionPoint.x >= pointB1.x)
						{
							if (intersectionPoint.y >= pointA1.y && intersectionPoint.y <= pointB1.y ||
								intersectionPoint.y <= pointA1.y && intersectionPoint.y >= pointB1.y)
							{
								collPoints.HasCollision = true;
							}
						}
					}
				}
			}
		}
	}

	return collPoints;
}
