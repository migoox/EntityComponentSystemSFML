#include "CollisionDetectionAlgorithms.h"
#include "Colliders/CircleCollider.h"
#include "Colliders/PlaneCollider.h"
#include "Colliders/PolygonCollider.h"
#include "../Game.h"

// TO DO: implement circle and plane
sf::Vector2f Basic::CollisionHelpers::FindFurthestPointCircle(sf::Vector2f globalCenter, float radius, sf::Vector2f direction)
{
	return sf::Vector2f();
}

sf::Vector2f Basic::CollisionHelpers::FindFurthestPointPlane(sf::Vector2f globalCenter, float distance, sf::Vector2f direction)
{
	return sf::Vector2f();
}

sf::Vector2f Basic::CollisionHelpers::FindFurthestPointPolygon(const std::vector<sf::Vector2f>* polygon, sf::Vector2f direction)
{
	assert(polygon->size() >= 3 && "PhysicsEngine: Can't find furthest point, since polygon is incorrectly defined.");

	using MathFunctions::Dot;

	// prepare values
	sf::Vector2f maxPoint = polygon->at(0);
	float maxDistance = Dot(maxPoint, direction);

	// find max point
	for (int i = 1; i < polygon->size(); i++)
	{
		float distance = Dot(polygon->at(i), direction);
		if (distance > maxDistance)
		{
			maxDistance = distance;
			maxPoint = polygon->at(i);
		}
	}

	return maxPoint;
}

sf::Vector2f Basic::CollisionHelpers::FindFurthestPointTriangle(const std::array<sf::Vector2f, 3>* triangle, sf::Vector2f direction)
{
	using MathFunctions::Dot;

	// prepare values
	sf::Vector2f maxPoint = triangle->at(0);
	float maxDistance = Dot(maxPoint, direction);

	// find max point
	for (int i = 1; i < 3; i++)
	{
		float distance = Dot(triangle->at(i), direction);
		if (distance > maxDistance)
		{
			maxDistance = distance;
			maxPoint = triangle->at(i);
		}
	}

	return maxPoint;
}


bool Basic::CollisionHelpers::SATAlgorithm(
	const std::vector<sf::Vector2f>* polygon1, 
	const std::vector<sf::Vector2f>* polygon2)
{
	// SAT algorithm
	using MathFunctions::Dot;
	const std::vector<sf::Vector2f>* poly1 = polygon1;
	const std::vector<sf::Vector2f>* poly2 = polygon2;

	for (int shape = 0; shape < 2; shape++)
	{
		if (shape == 1) // swap to second
		{
			poly1 = polygon2;
			poly2 = polygon1;
		}

		// iterate trough all of the vertices of current shape
		for (int currVertex = 0; currVertex < poly1->size(); currVertex++)
		{
			// find next vertex, if current vertex is the last, the next will be the first vertex in vector
			int nextVertex = (currVertex + 1) % poly1->size(); 

			// get points
			sf::Vector2f curr = poly1->at(currVertex);
			sf::Vector2f next = poly1->at(nextVertex);

			// create vector currVertex -> nextVertex
			sf::Vector2f currNextVec = next - curr;

			// create axis(normal of current edge)
			sf::Vector2f axis = sf::Vector2f(-currNextVec.y, currNextVec.x) / MathFunctions::VectorDistance(currNextVec);

			// find min and max 1d points(on axis) for shape 1
			float min1 = Dot(poly1->at(0), axis), max1 = Dot(poly1->at(0), axis);

			for (int i = 1; i < poly1->size(); i++)
			{
				float dot = Dot(poly1->at(i), axis);

				min1 = std::min(min1, dot);
				max1 = std::max(max1, dot);
			}

			// find min and max 1d points(on axis) for shape 2
			float min2 = Dot(poly2->at(0), axis), max2 = Dot(poly2->at(0), axis);

			for (int i = 1; i < poly2->size(); i++)
			{
				float dot = Dot(poly2->at(i), axis);

				min2 = std::min(min2, dot);
				max2 = std::max(max2, dot);
			}

			if (!(max2 >= min1 && max1 >= min2))
			{
				// there is no collision
				return false;
			}
		}
	}

	// there is collision
	return true;
}

Basic::CollisionPoints Basic::CollisionDetection::FindCircleCircleCollisionPoints(
	const CircleCollider* circle1, const Transform& transform1, 
	const CircleCollider* circle2, const Transform& transform2)
{
	sf::Vector2f globalPosition1 = circle1->GetGlobalCenterOfGravity(transform1);
	sf::Vector2f globalPosition2 = circle2->GetGlobalCenterOfGravity(transform2);

	float distanceBetweenCenters = sqrt(pow(globalPosition1.x - globalPosition2.x, 2) +
		pow(globalPosition1.y - globalPosition2.y, 2));

	CollisionPoints collPoints; // default parameters

	if (distanceBetweenCenters <= circle1->Radius() + circle2->Radius())
	{
		// collision
		collPoints.HasCollision = true;
		collPoints.Normal = MathFunctions::NormalizeVector(globalPosition1 - globalPosition2);
		collPoints.Depth = circle1->Radius() + circle2->Radius() - MathFunctions::Distance(globalPosition1, globalPosition2);
		collPoints.A = globalPosition1 - collPoints.Normal * circle1->Radius();
		collPoints.B = globalPosition2 + collPoints.Normal * circle2->Radius();
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
	sf::Vector2f center = circle->GetGlobalCenterOfGravity(circleTransform);

	// counting vectors
	sf::Vector2f PointACenterVector = center - pointA;

	sf::Vector2f ABVector = pointB - pointA;
	
	sf::Vector2f ShadowVector = MathFunctions::Dot(MathFunctions::NormalizeVector(ABVector), PointACenterVector) * MathFunctions::NormalizeVector(ABVector);

	sf::Vector2f CenterPlaneVector = ShadowVector - PointACenterVector;
	// create collision points
	CollisionPoints collPoints;

	if (MathFunctions::Distance(center, pointA) <= circle->Radius()) // one of the points of edge is inside the circle
	{
		collPoints.HasCollision = true;

		// counting collision points
		if (MathFunctions::VectorDistance(ShadowVector) >= plane->Distance() ||
			ABVector.x * ShadowVector.x + ABVector.y * ShadowVector.y <= 0) // if circle is "outside"
		{
			collPoints.B = pointA;
			collPoints.A = center + MathFunctions::NormalizeVector(collPoints.B - center) * circle->Radius();
			collPoints.Normal = MathFunctions::NormalizeVector(collPoints.B - collPoints.A);
			collPoints.Depth = MathFunctions::Distance(collPoints.A, collPoints.B);
		}
		else // if circle is between points A and B
		{
			collPoints.A = center + MathFunctions::NormalizeVector(CenterPlaneVector) * circle->Radius();
			collPoints.B = center + CenterPlaneVector;
			collPoints.Normal = MathFunctions::NormalizeVector(collPoints.B - collPoints.A);
			collPoints.Depth = MathFunctions::Distance(collPoints.A, collPoints.B);
		}
	}
	else if (MathFunctions::Distance(center, pointB) <= circle->Radius()) // one of the points of edge is inside the circle
	{
		collPoints.HasCollision = true;

		// counting collision points
		if (MathFunctions::VectorDistance(ShadowVector) >= plane->Distance() ||
			ABVector.x * ShadowVector.x + ABVector.y * ShadowVector.y <= 0) // if circle is "outside"
		{
			collPoints.B = pointB;
			collPoints.A = center + MathFunctions::NormalizeVector(collPoints.B - center) * circle->Radius();
			collPoints.Normal = MathFunctions::NormalizeVector(collPoints.B - collPoints.A);
			collPoints.Depth = MathFunctions::Distance(collPoints.A, collPoints.B);
		}
		else
		{
			collPoints.A = center + MathFunctions::NormalizeVector(CenterPlaneVector) * circle->Radius();
			collPoints.B = center + CenterPlaneVector;
			collPoints.Normal = MathFunctions::NormalizeVector(collPoints.B - collPoints.A);
			collPoints.Depth = MathFunctions::Distance(collPoints.A, collPoints.B);
		}
	}
	else // if circle collides with the line (not with it's points)
	{
		if (MathFunctions::VectorDistance(CenterPlaneVector) <= circle->Radius())
		{
			if (MathFunctions::VectorDistance(ShadowVector) <= plane->Distance() &&
				ABVector.x * ShadowVector.x + ABVector.y * ShadowVector.y >= 0)
			{
				collPoints.HasCollision = true;

				collPoints.A = center + MathFunctions::NormalizeVector(CenterPlaneVector) * circle->Radius();
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

Basic::CollisionPoints Basic::CollisionDetection::FindPolygonPolygonCollisionPoints(const PolygonCollider* polygon1, const Transform& polygon1Transform, const PolygonCollider* polygon2, const Transform& polygon2Transform)
{
	CollisionPoints collPoints;

	// for now i am going to handle only convex vs convex collision
	if (!polygon1->IsConvex() || !polygon2->IsConvex())
	{
		collPoints.HasCollision = false;
		collPoints.Resolvable = false;

		return collPoints;
	}

	// convex vs convex collision detection, without resolving
	// if one of the colliders is described as not to solve, use SAT Algorithm
	if (!polygon1->Solve || !polygon2->Solve)
	{
		collPoints.HasCollision = CollisionHelpers::SATAlgorithm(
			&polygon1->GlobalVertices(polygon1Transform), &polygon2->GlobalVertices(polygon2Transform));
		collPoints.Resolvable = false;
		return collPoints;
	}

	// convex vs convex collision detection, with resolving
	

	return CollisionPoints();
}

Basic::CollisionPoints Basic::CollisionDetection::FindPolygonCircleCollisionPoints(const PolygonCollider* polygon, const Transform& polygonTransform, const CircleCollider* circle, const Transform& circleTransform)
{
	return CollisionPoints();
}

Basic::CollisionPoints Basic::CollisionDetection::FindCirclePolygonCollisionPoints(const CircleCollider* circle, const Transform& circleTransform, const PolygonCollider* polygon, const Transform& polygonTransform)
{
	return CollisionPoints();
}

Basic::CollisionPoints Basic::CollisionDetection::FindPolygonPlaneCollisionPoints(const PolygonCollider* polygon, const Transform& polygonTransform, const PlaneCollider* plane, const Transform& planeTransform)
{
	return CollisionPoints();
}

Basic::CollisionPoints Basic::CollisionDetection::FindPlanePolygonCollisionPoints(const PlaneCollider* plane, const Transform& planeTransform, const PolygonCollider* polygon, const Transform& polygonTransform)
{
	return CollisionPoints();
}


