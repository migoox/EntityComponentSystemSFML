#include "CollisionDetectionAlgorithms.h"
#include "Colliders/CircleCollider.h"
#include "Colliders/PlaneCollider.h"
#include "../Game.h"

Basic::CollisionPoints Basic::CollisionDetection::FindCircleCircleCollisionPoints(
	const CircleCollider* circle1, const Transform& transform1, 
	const CircleCollider* circle2, const Transform& transform2)
{
	sf::Vector2f globalPosition1 = circle1->GetGlobalCenter(transform1);
	sf::Vector2f globalPosition2 = circle2->GetGlobalCenter(transform2);

	float distanceBetweenCenters = sqrt(pow(globalPosition1.x - globalPosition2.x, 2) +
		pow(globalPosition1.y - globalPosition2.y, 2));

	CollisionPoints collPoints; // default parameters

	if (distanceBetweenCenters <= circle1->Radius + circle2->Radius)
	{
		// collision
		collPoints.HasCollision = true;
		collPoints.Normal = MathFunctions::NormalizeVector(globalPosition1 - globalPosition2);
		collPoints.Depth = circle1->Radius + circle2->Radius - MathFunctions::Distance(globalPosition1, globalPosition2);
		collPoints.A = globalPosition1 - collPoints.Normal * circle1->Radius;
		collPoints.B = globalPosition2 + collPoints.Normal * circle2->Radius;
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
	sf::Vector2f PointACenterVector = center - pointA;

	sf::Vector2f ABVector = pointB - pointA;
	
	sf::Vector2f ShadowVector = MathFunctions::DotOfVectors(MathFunctions::NormalizeVector(ABVector), PointACenterVector) * MathFunctions::NormalizeVector(ABVector);

	sf::Vector2f CenterPlaneVector = ShadowVector - PointACenterVector;
	// create collision points
	CollisionPoints collPoints;

	if (MathFunctions::Distance(center, pointA) <= circle->Radius) // one of the points of edge is inside the circle
	{
		collPoints.HasCollision = true;

		// counting collision points
		if (MathFunctions::VectorDistance(ShadowVector) >= plane->Distance ||
			ABVector.x * ShadowVector.x + ABVector.y * ShadowVector.y <= 0) // if circle is "outside"
		{
			collPoints.B = pointA;
			collPoints.A = center + MathFunctions::NormalizeVector(collPoints.B - center) * circle->Radius;
			collPoints.Normal = MathFunctions::NormalizeVector(collPoints.B - collPoints.A);
			collPoints.Depth = MathFunctions::Distance(collPoints.A, collPoints.B);
		}
		else // if circle is between points A and B
		{
			collPoints.A = center + MathFunctions::NormalizeVector(CenterPlaneVector) * circle->Radius;
			collPoints.B = center + CenterPlaneVector;
			collPoints.Normal = MathFunctions::NormalizeVector(collPoints.B - collPoints.A);
			collPoints.Depth = MathFunctions::Distance(collPoints.A, collPoints.B);
		}
	}
	else if (MathFunctions::Distance(center, pointB) <= circle->Radius) // one of the points of edge is inside the circle
	{
		collPoints.HasCollision = true;

		// counting collision points
		if (MathFunctions::VectorDistance(ShadowVector) >= plane->Distance ||
			ABVector.x * ShadowVector.x + ABVector.y * ShadowVector.y <= 0) // if circle is "outside"
		{
			collPoints.B = pointB;
			collPoints.A = center + MathFunctions::NormalizeVector(collPoints.B - center) * circle->Radius;
			collPoints.Normal = MathFunctions::NormalizeVector(collPoints.B - collPoints.A);
			collPoints.Depth = MathFunctions::Distance(collPoints.A, collPoints.B);
		}
		else
		{
			collPoints.A = center + MathFunctions::NormalizeVector(CenterPlaneVector) * circle->Radius;
			collPoints.B = center + CenterPlaneVector;
			collPoints.Normal = MathFunctions::NormalizeVector(collPoints.B - collPoints.A);
			collPoints.Depth = MathFunctions::Distance(collPoints.A, collPoints.B);
		}
	}
	else // if circle collides with the line (not with it's points)
	{
		if (MathFunctions::VectorDistance(CenterPlaneVector) <= circle->Radius)
		{
			if (MathFunctions::VectorDistance(ShadowVector) <= plane->Distance &&
				ABVector.x * ShadowVector.x + ABVector.y * ShadowVector.y >= 0)
			{
				collPoints.HasCollision = true;

				collPoints.A = center + MathFunctions::NormalizeVector(CenterPlaneVector) * circle->Radius;
				collPoints.B = center + CenterPlaneVector;
				collPoints.Normal = MathFunctions::NormalizeVector(collPoints.B - collPoints.A);
				collPoints.Depth = MathFunctions::Distance(collPoints.A, collPoints.B);
			}
		}
	}

	return collPoints;
}

Basic::CollisionPoints Basic::CollisionDetection::FindPlaneCircleCollisionPoints(const PlaneCollider* plane, const Transform& planeTransform, const CircleCollider* circle, const Transform& circleTransform)
{
	CollisionPoints collPoints = FindCirclePlaneCollisionPoints(circle, circleTransform, plane, planeTransform);

	std::swap(collPoints.A, collPoints.B);
	collPoints.Normal *= (-1.0f);

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
