#include "CollisionDetectionAlgorithms.h"
#include "Colliders/CircleCollider.h"
#include "Colliders/PlaneCollider.h"
#include "Colliders/PolygonCollider.h"
#include "../Game.h"

bool Basic::GJK::HandleSimplex(
	Simplex& points, sf::Vector2f& direction)
{
	// notice that we have to use reference, since direction and 
	// points will be modified

	// if simplex contains 2 points, that means that it is a line
	// we have to find next direction, and return false, since in line case
	// there is no collision detection
	if (points.Size() == 2)
		return LineCase(points, direction);

	// if simplex contains 3 points, that means that it is a triangle and
	// we have to find out in which region lies origin
	return TriangleCase(points, direction);
}

bool Basic::GJK::LineCase(
	Simplex& points, sf::Vector2f& direction)
{
	using MathFunctions::TripleProduct;
	using MathFunctions::Dot;

	// get required points
	// in our data structure, first point in the row 
	// is recently added point
	sf::Vector2f a = points[0];
	sf::Vector2f b = points[1];

	// count vectors
	sf::Vector2f abVec = b - a;
	sf::Vector2f aoVec = sf::Vector2f(0.0f, 0.0f) - a; // origin - a

	// count ab perpendicular vector in correct direction(to origin)
	// using triple product
	sf::Vector2f abVecPerpendicular = TripleProduct(abVec, aoVec, abVec);

	if (Dot(abVec, aoVec) > 0.0f)
	{
		// set our direction
		direction = MathFunctions::NormalizeVector(abVecPerpendicular);
	}
	else
	{
		points = { a };
		direction = MathFunctions::NormalizeVector(aoVec);
	}

	// return false as there is no possibility to find out
	// if collision occured in line case
	return false;
}

bool Basic::GJK::TriangleCase(
	Simplex& points, sf::Vector2f& direction)
{
	using MathFunctions::TripleProduct;
	using MathFunctions::Dot;
	using MathFunctions::Cross;

	// get requied points
	// in our data structure, first point in the row
	// is recently added point
	sf::Vector2f a = points[0];
	sf::Vector2f b = points[1];
	sf::Vector2f c = points[2];

	// count vectors
	sf::Vector2f abVec = b - a;
	sf::Vector2f acVec = c - a;
	sf::Vector2f aoVec = sf::Vector2f(0.0f, 0.0f) - a; // origin - a

	// count ab perpendicular vector in correct direction(to origin)
	sf::Vector2f abVecPerpendicular =
		MathFunctions::TripleProduct(acVec, abVec, abVec);

	// count ac perpendicular vector in correct direction(to origin)
	sf::Vector2f acVecPerpendicular =
		MathFunctions::TripleProduct(abVec, acVec, acVec);

	// if origin lies in AB region
	if (Dot(abVecPerpendicular, aoVec) > 0.0f)
	{
		// get rid off c point, there will be found new point in
		// main loop, since we are changing direction as well
		points = { a, b };

		// set direction
		direction = MathFunctions::NormalizeVector(abVecPerpendicular);

		return false;
	}
	// if origin lies in AC region
	else if (Dot(acVecPerpendicular, aoVec) > 0.0f)
	{
		// get rid off b point, there will be found new point in
		// main loop, since we are changing direction as well
		points = { a, c };

		// set direction
		direction = MathFunctions::NormalizeVector(acVecPerpendicular);

		return false;
	}

	// origin is in the region ABC and collision is detected
	return true;
}

sf::Vector2f Basic::GJK::FindFurthestPointTriangle(
	const std::array<sf::Vector2f, 3>* triangle,
	sf::Vector2f direction)
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

sf::Vector2f Basic::GJK::SupportFunction(
	const ColliderItem* colliderA, const Transform& transformA,
	const ColliderItem* colliderB, const Transform& transformB,
	sf::Vector2f direction)
{
	return colliderA->FindFurthestPointInDirection(transformA, direction)
		- colliderB->FindFurthestPointInDirection(transformB, -direction);
}

Basic::CollisionPoints Basic::GJK::AlgorithmWithEPA(
	const ColliderItem* colliderA, const Transform& transformA,
	const ColliderItem* colliderB, const Transform& transformB)
{
	using MathFunctions::Dot;

	CollisionPoints collPoints;
	collPoints.HasCollision = false;

	// first direction - vector from center of A to B
	sf::Vector2f direction = MathFunctions::NormalizeVector(
		colliderB->GetGlobalCenterOfGravity(transformB) -
		colliderA->GetGlobalCenterOfGravity(transformA));

	// creating simplex and pushing first point based
	// on direction counted above
	// simplex is a data structure with max_size = 3
	Simplex simplex;
	simplex.PushFront(
		SupportFunction(colliderA, transformA,
			colliderB, transformB,
			direction));

	// changing direction to be towards the origin ( (0, 0) point )
	direction = MathFunctions::NormalizeVector(
		sf::Vector2f(0.0f, 0.0f) - simplex[0]);

	while (true)
	{
		// creating new point based on current direction
		sf::Vector2f A = SupportFunction(colliderA, transformA,
			colliderB, transformB,
			direction);

		// if point is not the proper region,
		// end algorithm with false
		if (Dot(A, direction) < 0.0f)
		{
			// there is no collision
			return collPoints;
		}

		// if point is in proper region,
		// add to simplex
		simplex.PushFront(A);

		// pass current state of simplex and current direction
		// to function, which will manage them and give results
		if (HandleSimplex(simplex, direction))
		{
			// triangle(simplex) which contains point (0, 0) is found,
			// so that collision occured

			collPoints = EPA::GetMTV(colliderA, transformA,
				colliderB, transformB, simplex);
			collPoints.HasCollision = true;

			return collPoints;
		}
	}

	// shouldn't happen
	return collPoints;
}

bool Basic::GJK::Algorithm(
	const ColliderItem* colliderA, const Transform& transformA,
	const ColliderItem* colliderB, const Transform& transformB)
{
	using MathFunctions::Dot;

	// first direction - vector from center of A to B
	sf::Vector2f direction = MathFunctions::NormalizeVector(
		colliderB->GetGlobalCenterOfGravity(transformB) -
		colliderA->GetGlobalCenterOfGravity(transformA));

	// creating simplex and pushing first point based
	// on direction counted above
	// simplex is a data structure with max_size = 3
	Simplex simplex;
	simplex.PushFront(
		SupportFunction(colliderA, transformA,
			colliderB, transformB,
			direction));

	// changing direction to be towards the origin ( (0, 0) point )
	direction = MathFunctions::NormalizeVector(
		sf::Vector2f(0.0f, 0.0f) - simplex[0]);

	while (true)
	{
		// creating new point based on current direction
		sf::Vector2f A = SupportFunction(colliderA, transformA,
			colliderB, transformB,
			direction);

		// if point is not the proper region,
		// end algorithm with false
		if (Dot(A, direction) < 0.0f)
		{
			// there is no collision
			return false;
		}

		// if point is in proper region,
		// add to simplex
		simplex.PushFront(A);

		// pass current state of simplex and current direction
		// to function, which will manage them and give results
		if (HandleSimplex(simplex, direction))
		{
			// triangle(simplex) which contains point (0, 0) is found,
			// so that collision occured

			return true;
		}
	}

	// shouldn't happen
	return false;
}

Basic::EPA::EPAEdge Basic::EPA::FindClosestEdge(
	const std::vector<sf::Vector2f>& polytope)
{
	using MathFunctions::Dot;
	using MathFunctions::NormalizeVector;

	// prepare containers and values
	EPAEdge closestEdge;
	closestEdge.Distance = FLT_MAX;

	for (int i = 0; i < polytope.size(); i++)
	{
		EPAEdge currEdge;

		currEdge.A = polytope[i];
		currEdge.B = polytope[(i + 1) % polytope.size()];

		currEdge.AIndex = i;

		// count required vectors
		sf::Vector2f abVec = currEdge.B - currEdge.A;
		sf::Vector2f oaVec = currEdge.A - sf::Vector2f(0.0f, 0.0f); // a - origin

		// get normal of the ab line
		currEdge.Normal = NormalizeVector(sf::Vector2f(-abVec.y, abVec.x));

		// count distance between edge and origin
		currEdge.Distance = Dot(oaVec, currEdge.Normal);

		// winding check
		if (currEdge.Distance < 0.0f)
		{
			currEdge.Distance = -currEdge.Distance;
			currEdge.Normal = -currEdge.Normal;
		}

		// set closest edge
		if (closestEdge.Distance > currEdge.Distance)
		{
			closestEdge = currEdge;
		}
	}

	return closestEdge;
}

Basic::CollisionPoints Basic::EPA::GetMTV(const ColliderItem* colliderA, const Transform& transformA,
	const ColliderItem* colliderB, const Transform& transformB,
	const GJK::Simplex& simplex)
{
	using MathFunctions::Dot;

	CollisionPoints collPoints;

	// pass simplex to polytope's points container
	std::vector<sf::Vector2f> polytope(simplex.begin(), simplex.end());

	// prevents infinite loop if sth goes wrong
	size_t iterationsCount = 0;

	while (true)
	{
		if (iterationsCount > MAX_EPA_ITERATIONS)
		{
			break;
		}
		iterationsCount++;

		// find closest edge
		EPAEdge closestEdge = FindClosestEdge(polytope);

		// find minkowski's difference vertex in normal's edge direction
		// this support point will be called p
		sf::Vector2f p = GJK::SupportFunction(
			colliderA, transformA,
			colliderB, transformB,
			closestEdge.Normal);

		// prepare vector
		sf::Vector2f opVec = p - sf::Vector2f(0.0f, 0.0f); // p - origin

		// prepare support point's edge distance 
		float newDistance = Dot(opVec, closestEdge.Normal);

		// if we found proper coll points
		if (newDistance - closestEdge.Distance < TOLERANCE)
		{
			//  set coll points
			collPoints.HasCollision = true;
			collPoints.Normal = -closestEdge.Normal;
			collPoints.Depth = closestEdge.Distance;

			// end of algorithm
			return collPoints;
		}

		// if condition wasn't met, expand our polytope
		// pass new support point between A and B of curr closest edge
		polytope.insert(polytope.begin() + closestEdge.AIndex + 1, p);
	}

	// shouldn't happen
	return collPoints;
}

bool Basic::SAT::Algorithm(
	const std::vector<sf::Vector2f>& polygonA,
	const std::vector<sf::Vector2f>& polygonB)
{
	// SAT algorithm
	using MathFunctions::Dot;
	const std::vector<sf::Vector2f>* poly1 = &polygonA;
	const std::vector<sf::Vector2f>* poly2 = &polygonB;

	for (int shape = 0; shape < 2; shape++)
	{
		if (shape == 1) // swap to second
		{
			poly1 = &polygonB;
			poly2 = &polygonA;
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

bool Basic::SAT::Algorithm(const std::vector<sf::Vector2f>& polygon, const std::array<sf::Vector2f, 3>& triangle)
{
	// SAT algorithm
	using MathFunctions::Dot;

	// iterate trough all of the vertices of polygon
	for (int currVertex = 0; currVertex < polygon.size(); currVertex++)
	{
		// find next vertex, if current vertex is the last, the next will be the first vertex in vector
		int nextVertex = (currVertex + 1) % polygon.size();

		// get points
		sf::Vector2f curr = polygon[currVertex];
		sf::Vector2f next = polygon[nextVertex];

		// create vector currVertex -> nextVertex
		sf::Vector2f currNextVec = next - curr;

		// create axis(normal of current edge)
		sf::Vector2f axis = sf::Vector2f(-currNextVec.y, currNextVec.x) / MathFunctions::VectorDistance(currNextVec);

		// find min and max 1d points(on axis) for shape 1
		float min1 = Dot(polygon.at(0), axis), max1 = Dot(polygon[0], axis);

		for (int i = 1; i < polygon.size(); i++)
		{
			float dot = Dot(polygon[i], axis);

			min1 = std::min(min1, dot);
			max1 = std::max(max1, dot);
		}

		// find min and max 1d points(on axis) for shape 2
		float min2 = Dot(triangle[0], axis), max2 = Dot(triangle[0], axis);

		for (int i = 1; i < triangle.size(); i++)
		{
			float dot = Dot(triangle[i], axis);

			min2 = std::min(min2, dot);
			max2 = std::max(max2, dot);
		}

		if (!(max2 >= min1 && max1 >= min2))
		{
			// there is no collision
			return false;
		}
	}

	// iterate trhough all of the vertices of triangle
	for (int currVertex = 0; currVertex < triangle.size(); currVertex++)
	{
		// find next vertex, if current vertex is the last, the next will be the first vertex in vector
		int nextVertex = (currVertex + 1) % triangle.size();

		// get points
		sf::Vector2f curr = triangle[currVertex];
		sf::Vector2f next = triangle[nextVertex];

		// create vector currVertex -> nextVertex
		sf::Vector2f currNextVec = next - curr;

		// create axis(normal of current edge)
		sf::Vector2f axis = sf::Vector2f(-currNextVec.y, currNextVec.x) / MathFunctions::VectorDistance(currNextVec);

		// find min and max 1d points(on axis) for shape 1
		float min1 = Dot(triangle[0], axis), max1 = Dot(triangle[0], axis);

		for (int i = 1; i < triangle.size(); i++)
		{
			float dot = Dot(triangle[i], axis);

			min1 = std::min(min1, dot);
			max1 = std::max(max1, dot);
		}

		// find min and max 1d points(on axis) for shape 2
		float min2 = Dot(polygon[0], axis), max2 = Dot(polygon[0], axis);

		for (int i = 1; i < polygon.size(); i++)
		{
			float dot = Dot(polygon[i], axis);

			min2 = std::min(min2, dot);
			max2 = std::max(max2, dot);
		}

		if (!(max2 >= min1 && max1 >= min2))
		{
			// there is no collision
			return false;
		}
	}

	// there is collision
	return true;
}

bool Basic::SAT::Algorithm(const std::array<sf::Vector2f, 3>& triangleA, const std::array<sf::Vector2f, 3>& triangleB)
{
	// SAT algorithm
	using MathFunctions::Dot;
	const std::array<sf::Vector2f, 3>* poly1 = &triangleA;
	const std::array<sf::Vector2f, 3>* poly2 = &triangleB;

	for (int shape = 0; shape < 2; shape++)
	{
		if (shape == 1) // swap to second
		{
			poly1 = &triangleB;
			poly2 = &triangleA;
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

// COLLISION DETECTION FUNCTIONS

Basic::CollisionPoints Basic::CollisionDetection::FindCircleCircleCollisionPoints(
	const CircleCollider* circleA, const Transform& transformA,
	const CircleCollider* circleB, const Transform& transformB)
{
	sf::Vector2f globalPosition1 = circleA->GetGlobalCenterOfGravity(transformA);
	sf::Vector2f globalPosition2 = circleB->GetGlobalCenterOfGravity(transformB);

	float distanceBetweenCenters = sqrt(pow(globalPosition1.x - globalPosition2.x, 2) +
		pow(globalPosition1.y - globalPosition2.y, 2));

	CollisionPoints collPoints; // default parameters

	if (distanceBetweenCenters <= circleA->Radius() + circleB->Radius())
	{
		// collision
		collPoints.HasCollision = true;
		collPoints.Normal = MathFunctions::NormalizeVector(globalPosition1 - globalPosition2);
		collPoints.Depth = circleA->Radius() + circleB->Radius() - MathFunctions::Distance(globalPosition1, globalPosition2);
		collPoints.A = globalPosition1 - collPoints.Normal * circleA->Radius();
		collPoints.B = globalPosition2 + collPoints.Normal * circleB->Radius();
	}

	return collPoints;
}

Basic::CollisionPoints Basic::CollisionDetection::FindCirclePlaneCollisionPoints(
	const CircleCollider* circleA, const Transform& circleATransform,
	const PlaneCollider* planeB, const Transform& planeBTransform)
{
	/*float pi = 3.141592653f;

	// counting points
	sf::Vector2f pointA = planeB->GetGlobalAPoint(planeBTransform);
	sf::Vector2f pointB = planeB->GetGlobalBPoint(planeBTransform);
	sf::Vector2f center = circleA->GetGlobalCenterOfGravity(circleATransform);

	// counting vectors
	sf::Vector2f PointACenterVector = center - pointA;

	sf::Vector2f ABVector = pointB - pointA;

	sf::Vector2f ShadowVector = MathFunctions::Dot(MathFunctions::NormalizeVector(ABVector), PointACenterVector) * MathFunctions::NormalizeVector(ABVector);

	sf::Vector2f CenterPlaneVector = ShadowVector - PointACenterVector;
	// create collision points
	CollisionPoints collPoints;

	if (MathFunctions::Distance(center, pointA) <= circleA->Radius()) // one of the points of edge is inside the circle
	{
		collPoints.HasCollision = true;

		// counting collision points
		if (MathFunctions::VectorDistance(ShadowVector) >= planeB->Distance() ||
			ABVector.x * ShadowVector.x + ABVector.y * ShadowVector.y <= 0) // if circle is "outside"
		{
			collPoints.B = pointA;
			collPoints.A = center + MathFunctions::NormalizeVector(collPoints.B - center) * circleA->Radius();
			collPoints.Normal = MathFunctions::NormalizeVector(collPoints.B - collPoints.A);
			collPoints.Depth = MathFunctions::Distance(collPoints.A, collPoints.B);
		}
		else // if circle is between points A and B
		{
			collPoints.A = center + MathFunctions::NormalizeVector(CenterPlaneVector) * circleA->Radius();
			collPoints.B = center + CenterPlaneVector;
			collPoints.Normal = MathFunctions::NormalizeVector(collPoints.B - collPoints.A);
			collPoints.Depth = MathFunctions::Distance(collPoints.A, collPoints.B);
		}
	}
	else if (MathFunctions::Distance(center, pointB) <= circleA->Radius()) // one of the points of edge is inside the circle
	{
		collPoints.HasCollision = true;

		// counting collision points
		if (MathFunctions::VectorDistance(ShadowVector) >= planeB->Distance() ||
			ABVector.x * ShadowVector.x + ABVector.y * ShadowVector.y <= 0) // if circle is "outside"
		{
			collPoints.B = pointB;
			collPoints.A = center + MathFunctions::NormalizeVector(collPoints.B - center) * circleA->Radius();
			collPoints.Normal = MathFunctions::NormalizeVector(collPoints.B - collPoints.A);
			collPoints.Depth = MathFunctions::Distance(collPoints.A, collPoints.B);
		}
		else
		{
			collPoints.A = center + MathFunctions::NormalizeVector(CenterPlaneVector) * circleA->Radius();
			collPoints.B = center + CenterPlaneVector;
			collPoints.Normal = MathFunctions::NormalizeVector(collPoints.B - collPoints.A);
			collPoints.Depth = MathFunctions::Distance(collPoints.A, collPoints.B);
		}
	}
	else // if circle collides with the line (not with it's points)
	{
		if (MathFunctions::VectorDistance(CenterPlaneVector) <= circleA->Radius())
		{
			if (MathFunctions::VectorDistance(ShadowVector) <= planeB->Distance() &&
				ABVector.x * ShadowVector.x + ABVector.y * ShadowVector.y >= 0)
			{
				collPoints.HasCollision = true;

				collPoints.A = center + MathFunctions::NormalizeVector(CenterPlaneVector) * circleA->Radius();
				collPoints.B = center + CenterPlaneVector;
				collPoints.Normal = MathFunctions::NormalizeVector(collPoints.B - collPoints.A);
				collPoints.Depth = MathFunctions::Distance(collPoints.A, collPoints.B);
			}
		}
	}*/

	CollisionPoints collPoints;

	// collision detection without resolution
	// if one of the colliders is described as not to solve
	if (!circleA->Solve || !planeB->Solve)
	{
		/*collPoints.HasCollision = SAT::Algorithm(&polygon1->GlobalVertices(polygon1Transform),
			&polygon2->GlobalVertices(polygon2Transform));*/

		collPoints.HasCollision = GJK::Algorithm(circleA, circleATransform,
			planeB, planeBTransform);

		collPoints.Resolvable = false;

		return collPoints;
	}

	// collision detection, with resolution
	collPoints = GJK::AlgorithmWithEPA(circleA, circleATransform,
		planeB, planeBTransform);

	return collPoints;
}

Basic::CollisionPoints Basic::CollisionDetection::FindPlaneCircleCollisionPoints(
	const PlaneCollider* planeA, const Transform& planeATransform,
	const CircleCollider* circleB, const Transform& circleBTransform)
{
	CollisionPoints collPoints = FindCirclePlaneCollisionPoints(circleB, circleBTransform, planeA, planeATransform);

	std::swap(collPoints.A, collPoints.B);
	collPoints.Normal *= (-1.0f);

	return collPoints;
}

Basic::CollisionPoints Basic::CollisionDetection::FindPlanePlaneCollisionPoints(
	const PlaneCollider* planeA, const Transform& planeATransform,
	const PlaneCollider* planeB, const Transform& planeBTransform)
{
	float pi = 3.141592653f;

	// counting points
	sf::Vector2f pointA1 = planeA->GetGlobalAPoint(planeATransform);
	sf::Vector2f pointB1 = planeA->GetGlobalBPoint(planeATransform);
	sf::Vector2f pointA2 = planeB->GetGlobalAPoint(planeBTransform);
	sf::Vector2f pointB2 = planeB->GetGlobalBPoint(planeBTransform);

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

Basic::CollisionPoints Basic::CollisionDetection::FindPolygonPolygonCollisionPoints(
	const PolygonCollider* polygonA, const Transform& polygonATransform,
	const PolygonCollider* polygonB, const Transform& polygonBTransform)
{
	using MathFunctions::Dot;
	using MathFunctions::NormalizeVector;

	CollisionPoints collPoints;

	// convex collision detection (without resolving it)
	if (!polygonA->IsConvex())
	{
		collPoints.HasCollision = false;
		collPoints.Resolvable = false;

		auto& triangles = polygonA->GlobalTriangles(polygonATransform);
		auto& vertices = polygonB->GlobalVertices(polygonBTransform);
		for (auto& triangle : triangles)
		{
			if (SAT::Algorithm(vertices, triangle))
			{
				collPoints.HasCollision = true;
				break;
			}
		}

		return collPoints;
	}
	else if (!polygonB->IsConvex())
	{
		collPoints.HasCollision = false;
		collPoints.Resolvable = false;

		auto& triangles = polygonB->GlobalTriangles(polygonBTransform);
		auto& vertices = polygonA->GlobalVertices(polygonATransform);
		for (auto& triangle : triangles)
		{
			if (SAT::Algorithm(vertices, triangle))
			{
				collPoints.HasCollision = true;
				break;
			}
		}

		return collPoints;
	}
	else if(!polygonA->IsConvex() && !polygonB->IsConvex()) // both colliders aren't convex
	{
		collPoints.HasCollision = false;
		collPoints.Resolvable = false;

		auto& trianglesA = polygonA->GlobalTriangles(polygonATransform);
		auto& trianglesB = polygonB->GlobalTriangles(polygonBTransform);

		for (auto& triangleA : trianglesA)
		{
			for (auto& triangleB : trianglesB)
			{
				if (SAT::Algorithm(triangleA, triangleB))
				{
					collPoints.HasCollision = true;
					break;
				}
			}
		}

		return collPoints;
	}

	// convex vs convex collision detection, without resolving
	// if one of the colliders is described as not to solve, use SAT Algorithm
	if (!polygonA->Solve || !polygonA->Solve)
	{
		/*collPoints.HasCollision = SAT::Algorithm(&polygon1->GlobalVertices(polygon1Transform),
			&polygon2->GlobalVertices(polygon2Transform));*/

		collPoints.HasCollision = GJK::Algorithm(polygonA, polygonATransform,
			polygonB, polygonBTransform);

		collPoints.Resolvable = false;

		return collPoints;
	}

	// convex vs convex collision detection, with resolving
	collPoints = GJK::AlgorithmWithEPA(polygonA, polygonATransform,
		polygonB, polygonBTransform);

	return collPoints;

	/*
	// if there is no collision - end function
	if (!collPoints.HasCollision)
		return collPoints;
	*/

	// else - get collision contact points(manifold points)

	// getting collision contact points using clipping method

	// clipping collision normal
	
	// when we have 2 colliding objects, collPoints.Normal shows direction
	// in which A object should be moved to fix that collision,
	// but in clipping we need normal which points from A shape to B shape
	// so that we have to reverse current normal 

	//f::Vector2f normal = -collPoints.Normal;

	// find the best edges
	// note that normal goes from A body to B, so we have to reverse normal for polygon B
	/*ClippingAlgo::CPEdge ref = polygonA->GetTheBestClippingEdge(polygonATransform, normal);
	ClippingAlgo::CPEdge inc = polygonB->GetTheBestClippingEdge(polygonBTransform, -normal);
	
	bool flip = false; // if A is inc and B is ref than we will have to flip normal in further code

	sf::Vector2f refVec = NormalizeVector(ref.B - ref.A);
	sf::Vector2f incVec = NormalizeVector(inc.B - inc.A);

	// swap ref and inc if it is necessary
	if (std::abs(Dot(refVec, normal)) > std::abs(Dot(incVec, normal)))
	{
		std::swap(ref, inc);
		std::swap(refVec, incVec);

		flip = true;
	}

	// clipping time

	// first clip
	ClippingAlgo::ClippedPoints clippedPoinst = ClippingAlgo::Clip(inc.A, inc.B, refVec, Dot(refVec, ref.A));

	// if clipping failed return unresolvable collision result
	if (clippedPoinst.Size < 2)
	{
		//return collPoints;
	}

	// second clip
	clippedPoinst = ClippingAlgo::Clip(clippedPoinst.Points[0], clippedPoinst.Points[1], -refVec, -Dot(refVec, ref.B));

	// if clipping failed return unresolvable collision result
	if (clippedPoinst.Size < 2)
	{
		//return collPoints;
	}

	if (flip)
		ref.Normal = -ref.Normal;
	
	float max = Dot(ref.Normal, ref.Max);

	if (Dot(ref.Normal, clippedPoinst.Points[0]) - max < 0.0f)
	{
		clippedPoinst.Size--;
		clippedPoinst.Points[0] = clippedPoinst.Points[1];
	}

	if (Dot(ref.Normal, clippedPoinst.Points[1]) - max < 0.0f)
	{
		clippedPoinst.Size--;
	}
	/*Line refLine(ref.A, ref.B, 4.0f, sf::Color(200.0f, 0.0f, 50.0f, 160.0f));
	Line incLine(inc.A, inc.B, 4.0f, sf::Color(0.0f, 200.0f, 50.0f, 160.0f));

	VisualGizmos::DrawOnce(refLine);
	VisualGizmos::DrawOnce(incLine);*/




	/*sf::VertexArray arr(sf::PrimitiveType::Lines, 2);

	arr[0].position = polygonATransform.getPosition();
	arr[1].position = polygonATransform.getPosition() - collPoints.Normal * collPoints.Depth;

	arr[0].color = sf::Color::Black;
	arr[1].color = sf::Color::Green;

	VisualGizmos::DrawOnce(arr);

	if (clippedPoinst.Size == 1)
	{
		CircleShape circle(2.0f);
		circle.setFillColor(sf::Color(255.0f, 0.0f, 255.0f, 0.8f * 255.0f));
		circle.setOrigin(1.0f, 1.0f);
		circle.setPosition(clippedPoinst.Points[0]);

		VisualGizmos::DrawOnce(circle);
	}
	else if (clippedPoinst.Size == 2)
	{
		CircleShape circle(2.0f);
		circle.setFillColor(sf::Color(255.0f, 0.0f, 255.0f, 0.8f * 255.0f));
		circle.setOrigin(1.0f, 1.0f);
		circle.setPosition(clippedPoinst.Points[0]);

		CircleShape circle2(2.0f);
		circle2.setFillColor(sf::Color(255.0f, 0.0f, 255.0f, 0.8f * 255.0f));
		circle2.setOrigin(1.0f, 1.0f);
		circle2.setPosition(clippedPoinst.Points[1]);

		VisualGizmos::DrawOnce(circle2);
		VisualGizmos::DrawOnce(circle);
	}
	else
	{
		//std::cout << "no points\n" << Game::DeltaTime().asSeconds() << std::endl;
	}
	
	/*CircleShape circle(2.0f);
	circle.setFillColor(sf::Color(255.0f, 0.0f, 255.0f, 0.8f * 255.0f));
	circle.setOrigin(1.0f, 1.0f);
	circle.setPosition(ref.A);

	CircleShape circle2(2.0f);
	circle2.setFillColor(sf::Color(255.0f, 0.0f, 255.0f, 0.8f * 255.0f));
	circle2.setOrigin(1.0f, 1.0f);
	circle2.setPosition(ref.B);

	VisualGizmos::DrawOnce(circle);
	VisualGizmos::DrawOnce(circle2);
	*/

	return collPoints;
}

Basic::CollisionPoints Basic::CollisionDetection::FindPolygonCircleCollisionPoints(
	const PolygonCollider* polygonA, const Transform& polygonATransform,
	const CircleCollider* circleB, const Transform& circleBTransform)
{
	CollisionPoints collPoints;

	// concave polygon vs circle, whitout resolution
	if (!polygonA->IsConvex())
	{
		collPoints.HasCollision = false;
		collPoints.Resolvable = false;

		return collPoints;
	}


	// collision detection without resolution
	// if one of the colliders is described as not to solve
	if (!polygonA->Solve || !circleB->Solve)
	{
		/*collPoints.HasCollision = SAT::Algorithm(&polygon1->GlobalVertices(polygon1Transform),
			&polygon2->GlobalVertices(polygon2Transform));*/

		collPoints.HasCollision = GJK::Algorithm(polygonA, polygonATransform,
			circleB, circleBTransform);

		collPoints.Resolvable = false;

		return collPoints;
	}

	// collision detection, with resolution
	collPoints = GJK::AlgorithmWithEPA(polygonA, polygonATransform,
		circleB, circleBTransform);

	return collPoints;
}

Basic::CollisionPoints Basic::CollisionDetection::FindCirclePolygonCollisionPoints(
	const CircleCollider* circleA, const Transform& circleATransform,
	const PolygonCollider* polygonB, const Transform& polygonBTransform)
{
	CollisionPoints collPoints = FindPolygonCircleCollisionPoints(
		polygonB, polygonBTransform, 
		circleA, circleATransform);

	std::swap(collPoints.A, collPoints.B);
	collPoints.Normal = -collPoints.Normal;
	
	return collPoints;
}

Basic::CollisionPoints Basic::CollisionDetection::FindPolygonPlaneCollisionPoints(
	const PolygonCollider* polygonA, const Transform& polygonATransform,
	const PlaneCollider* planeB, const Transform& planeBTransform)
{
	CollisionPoints collPoints;

	// concave polygon vs circle collision detection without resolution
	if (!polygonA->IsConvex())
	{
		collPoints.HasCollision = false;
		collPoints.Resolvable = false;

		return collPoints;
	}


	// collision detection without resolution
	// if one of the colliders is described as not to solve
	if (!polygonA->Solve || !planeB->Solve)
	{
		/*collPoints.HasCollision = SAT::Algorithm(&polygon1->GlobalVertices(polygon1Transform),
			&polygon2->GlobalVertices(polygon2Transform));*/

		collPoints.HasCollision = GJK::Algorithm(polygonA, polygonATransform,
			planeB, planeBTransform);

		collPoints.Resolvable = false;

		return collPoints;
	}

	// collision detection, with resolution
	collPoints = GJK::AlgorithmWithEPA(polygonA, polygonATransform,
		planeB, planeBTransform);

	return collPoints;
}

Basic::CollisionPoints Basic::CollisionDetection::FindPlanePolygonCollisionPoints(
	const PlaneCollider* planeA, const Transform& planeATransform,
	const PolygonCollider* polygonB, const Transform& polygonBTransform)
{
	CollisionPoints collPoints = FindPolygonPlaneCollisionPoints(
		polygonB, polygonBTransform,
		planeA, planeATransform);

	std::swap(collPoints.A, collPoints.B);
	collPoints.Normal = -collPoints.Normal;

	return collPoints;
}

Basic::ClippingAlgo::ClippedPoints Basic::ClippingAlgo::Clip(
	const sf::Vector2f& vertex1, const sf::Vector2f& vertex2, const sf::Vector2f& refVec, float dotProduct)
{
	using MathFunctions::Dot;

	ClippedPoints clippedPoints;

	float dotProduct1 = Dot(vertex1, refVec) - dotProduct;
	float dotProduct2 = Dot(vertex2, refVec) - dotProduct;

	if (dotProduct1 >= 0.0f)
		clippedPoints.Points[clippedPoints.Size++] = vertex1;

	if (dotProduct2 >= 0.0f)
		clippedPoints.Points[clippedPoints.Size++] = vertex2;
			
	if (dotProduct1 * dotProduct2 < 0.0f)
	{
		sf::Vector2f edgeVec = vertex2 - vertex1;

		float location = dotProduct1 / (dotProduct1 - dotProduct2);

		edgeVec *= location;
		edgeVec += vertex1;

		clippedPoints.Points[clippedPoints.Size++] = edgeVec;
	}

	return clippedPoints;
}
