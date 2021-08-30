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
	// we have to find out in which region origin lies
	return TriangleCase(points, direction);
}

bool Basic::GJK::LineCase(
	Simplex& points, sf::Vector2f& direction)
{
	using Maths::TripleProduct;
	using Maths::Dot;

	// get required points
	// in our data structure, first point in the row 
	// is recently added point
	sf::Vector2f a = points[0];
	sf::Vector2f b = points[1];

	// count vectors
	sf::Vector2f abVec = b - a;
	sf::Vector2f aoVec = -a; // origin - a

	// count ab perpendicular vector in correct direction(to origin)
	// using triple product
	sf::Vector2f abVecPerpendicular = TripleProduct(abVec, aoVec, abVec);

	// origin lies in the direction of ab perp vector
	if (Dot(abVec, aoVec) > 0.0f)
	{
		// set new direction
		direction = Maths::NormalizeVector(abVecPerpendicular);
	}
	else
	{
		// origin lies either on edge or in the opposite direction of ab perp vector
		points = { a };
		direction = Maths::NormalizeVector(aoVec);
	}

	// return false as there is no possibility to find out
	// if collision occured in line case
	return false;
}

bool Basic::GJK::TriangleCase(
	Simplex& points, sf::Vector2f& direction)
{
	using Maths::TripleProduct;
	using Maths::NormalizeVector;
	using Maths::Dot;
	using Maths::Cross;
	
	// get requied points
	// in our data structure, first point in the simplex
	// is recently added point
	sf::Vector2f a = points[0];
	sf::Vector2f b = points[1];
	sf::Vector2f c = points[2];

	// count vectors
	sf::Vector2f abVec = b - a;
	sf::Vector2f acVec = c - a;
	sf::Vector2f aoVec = -a; // origin - a

	// count ab perpendicular vector in correct direction(to origin)
	sf::Vector2f abVecPerpendicular = 
		Maths::TripleProduct(acVec, abVec, abVec);

	// count ac perpendicular vector in correct direction(to origin)
	sf::Vector2f acVecPerpendicular = 
		Maths::TripleProduct(abVec, acVec, acVec);

	// if origin lies in AB region
	if (Dot(abVecPerpendicular, aoVec) > 0.0f)
	{
		// get rid off c point, there will be found new point in
		// main loop, since we are changing direction as well
		points = { a, b };

		// set direction
		direction = Maths::NormalizeVector(abVecPerpendicular);

		return false;
	}
	// if origin lies in AC region
	else if (Dot(acVecPerpendicular, aoVec) > 0.0f)
	{
		// get rid off b point, there will be found new point in
		// main loop, since we are changing direction as well
		points = { a, c };

		// set direction
		direction = Maths::NormalizeVector(acVecPerpendicular);

		return false;
	}

	// origin is in the region ABC(inside the triangle) and collision is detected*/
	return true;
}

sf::Vector2f Basic::GJK::FindFurthestPointTriangle(
	const std::array<sf::Vector2f, 3>* triangle,
	sf::Vector2f direction)
{
	using Maths::Dot;

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

Basic::CollisionManifold Basic::GJK::AlgorithmWithEPA(
	const ColliderItem* colliderA, const Transform& transformA,
	const ColliderItem* colliderB, const Transform& transformB)
{
	using Maths::Dot;

	CollisionManifold manifold;
	manifold.HasCollision = false;

	// first direction - vector from center of A to B
	sf::Vector2f direction = Maths::NormalizeVector(
		colliderB->GetGlobalCenterOfGravity(transformB) -
		colliderA->GetGlobalCenterOfGravity(transformA));

	// creating simplex and pushing first point based
	// on direction counted above
	// simplex is a data structure with max_size = 3
	Simplex simplex;
	simplex.PushFront(SupportFunction(colliderA, transformA,
									  colliderB, transformB, direction));

	// changing direction to be towards the origin ( (0, 0) point )
	direction = Maths::NormalizeVector(-simplex[0]); // direction = origin - simplex[0]

	while (true)
	{
		// creating new point based on current direction
		sf::Vector2f A = SupportFunction(colliderA, transformA,
			colliderB, transformB,
			direction);

		// if point is not the proper region,
		// finish algorithm with false result
		if (Dot(A, direction) <= 0.0f)
		{
			// there is no collision
			return manifold;
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
			// using EPA, minimum translation vector will be found
			 
			manifold = EPA::GetMTV(
				colliderA, transformA,
				colliderB, transformB, simplex);

			return manifold;
		}
	}

	// shouldn't happen
	return manifold;
}

bool Basic::GJK::Algorithm(
	const ColliderItem* colliderA, const Transform& transformA,
	const ColliderItem* colliderB, const Transform& transformB)
{
	using Maths::Dot;

	// first direction - vector from center of A to B
	sf::Vector2f direction = Maths::NormalizeVector(
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
	direction = Maths::NormalizeVector(
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
	using Maths::Dot;
	using Maths::NormalizeVector;

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

Basic::CollisionManifold Basic::EPA::GetMTV(const ColliderItem* colliderA, const Transform& transformA,
	const ColliderItem* colliderB, const Transform& transformB,
	const GJK::Simplex& simplex)
{
	using Maths::Dot;

	CollisionManifold manifold;

	// pass simplex to polytope's points container
	std::vector<sf::Vector2f> polytope(simplex.begin(), simplex.end());

	// prevents infinite loop if sth goes wrong
	size_t iterationsCount = 0;

	while (true)
	{
		if (iterationsCount > MAX_EPA_ITERATIONS)
		{
			// infinite loop
			manifold.Normal = sf::Vector2f(0.0f, 0.0f);
			manifold.Depth = 0.0f;
			return manifold;
		}
		iterationsCount++;

		// find closest edge
		EPAEdge closestEdge = FindClosestEdge(polytope);

		// find minkowski's difference vertex in normal's edge direction
		// this support point will be called p
		sf::Vector2f supportPoint = GJK::SupportFunction(
			colliderA, transformA,
			colliderB, transformB,
			closestEdge.Normal);

		// prepare vector

		// prepare support point's edge distance 
		// first argument is supportPoint - origin (origin->supportPoint vector)
		if(closestEdge.Distance < CollisionDetection::DEPTH_TOLERANCE)
		{
			// set coll points
			manifold.HasCollision = true;
			manifold.Normal = -closestEdge.Normal;
			manifold.Depth = 0.0f;

			// end of algorithm
			return manifold;
		}

		float newDistance = Dot(supportPoint, closestEdge.Normal); 

		// if we found proper coll points
		if (newDistance - closestEdge.Distance < TOLERANCE)
		{
			//  set coll points
			manifold.HasCollision = true;
			manifold.Normal = -closestEdge.Normal;
			manifold.Depth = closestEdge.Distance;

			// end of algorithm
			return manifold;
		}

		// if condition wasn't met, expand our polytope
		// pass new support point between A and B of curr closest edge
		polytope.insert(polytope.begin() + closestEdge.AIndex + 1, supportPoint);
	}

	// shouldn't happen
	return manifold;
}

bool Basic::SAT::Algorithm(
	const std::vector<sf::Vector2f>& polygonA,
	const std::vector<sf::Vector2f>& polygonB)
{
	// SAT algorithm
	using Maths::Dot;
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
			sf::Vector2f axis = sf::Vector2f(-currNextVec.y, currNextVec.x) / Maths::VectorDistance(currNextVec);

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
	using Maths::Dot;

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
		sf::Vector2f axis = sf::Vector2f(-currNextVec.y, currNextVec.x) / Maths::VectorDistance(currNextVec);

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
		sf::Vector2f axis = sf::Vector2f(-currNextVec.y, currNextVec.x) / Maths::VectorDistance(currNextVec);

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
	using Maths::Dot;
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
			sf::Vector2f axis = sf::Vector2f(-currNextVec.y, currNextVec.x) / Maths::VectorDistance(currNextVec);

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

Basic::CollisionManifold Basic::CollisionDetection::FindCircleCircleCollisionPoints(
	const CircleCollider* circleA, const Transform& transformA,
	const CircleCollider* circleB, const Transform& transformB)
{
	sf::Vector2f globalPosition1 = circleA->GetGlobalCenterOfGravity(transformA);
	sf::Vector2f globalPosition2 = circleB->GetGlobalCenterOfGravity(transformB);

	float distanceBetweenCenters = sqrt(pow(globalPosition1.x - globalPosition2.x, 2) +
		pow(globalPosition1.y - globalPosition2.y, 2));

	CollisionManifold manifold; // default parameters

	if (distanceBetweenCenters <= circleA->Radius() + circleB->Radius())
	{
		// collision
		manifold.HasCollision = true;
		manifold.Normal = Maths::NormalizeVector(globalPosition1 - globalPosition2);
		manifold.Depth = circleA->Radius() + circleB->Radius() - Maths::Distance(globalPosition1, globalPosition2);
		
		// objects are touching and not colliding
		if (manifold.Depth < DEPTH_TOLERANCE)
			manifold.Depth = 0.0f;

		if (manifold.Depth == 0.0f)
			return manifold;

		// find contact point
		ContactPoints contactPoints = 
		{ circleB->GetGlobalCenterOfGravity(transformB) + circleB->Radius() * manifold.Normal };

		// fix contact point
		if (circleB->Movable)
		{
			sf::Vector2f fixer = -manifold.Depth * manifold.Normal;

			if (circleA->Movable)
				fixer *= 0.5f;

			contactPoints[0] += fixer;

			CircleShape circle(2.0f);
			circle.setFillColor(sf::Color(255.0f, 0.0f, 255.0f, 0.8f * 255.0f));
			circle.setOrigin(2.0f, 2.0f);
			circle.setPosition(contactPoints[0]);

			VisualGizmos::DrawOnce(circle);
		}

		manifold.Points = contactPoints;
	}

	return manifold;
}

Basic::CollisionManifold Basic::CollisionDetection::FindCirclePlaneCollisionPoints(
	const CircleCollider* circleA, const Transform& circleATransform,
	const PlaneCollider* planeB, const Transform& planeBTransform)
{
	CollisionManifold manifold;

	// collision detection without resolution
	// if one of the colliders is described as not to solve
	if (!circleA->Solve || !planeB->Solve)
	{
		/*collPoints.HasCollision = SAT::Algorithm(&polygon1->GlobalVertices(polygon1Transform),
			&polygon2->GlobalVertices(polygon2Transform));*/

		manifold.HasCollision = GJK::Algorithm(circleA, circleATransform,
			planeB, planeBTransform);

		manifold.Resolvable = false;

		return manifold;
	}

	// collision detection, with resolution
	manifold = GJK::AlgorithmWithEPA(circleA, circleATransform,
		planeB, planeBTransform);

	if (!manifold.HasCollision)
		return manifold;

	if (manifold.Depth == 0.0f)
		return manifold;

	// find contact point
	ContactPoints contactPoints =
	{ circleA->GetGlobalCenterOfGravity(circleATransform) + circleA->Radius() * -manifold.Normal };

	// fix contact point
	if (circleA->Movable)
	{
		sf::Vector2f fixer = manifold.Depth * manifold.Normal;

		if (planeB->Movable)
			fixer *= 0.5f;

		contactPoints[0] += fixer;

		CircleShape circle(2.0f);
		circle.setFillColor(sf::Color(255.0f, 0.0f, 255.0f, 0.8f * 255.0f));
		circle.setOrigin(2.0f, 2.0f);
		circle.setPosition(contactPoints[0]);

		VisualGizmos::DrawOnce(circle);
	}

	manifold.Points = contactPoints;

	return manifold;
}

Basic::CollisionManifold Basic::CollisionDetection::FindPlaneCircleCollisionPoints(
	const PlaneCollider* planeA, const Transform& planeATransform,
	const CircleCollider* circleB, const Transform& circleBTransform)
{
	CollisionManifold manifold = FindCirclePlaneCollisionPoints(circleB, circleBTransform, planeA, planeATransform);

	std::swap(manifold.A, manifold.B);
	manifold.Normal *= (-1.0f);

	return manifold;
}

Basic::CollisionManifold Basic::CollisionDetection::FindPlanePlaneCollisionPoints(
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
	CollisionManifold manifold;
	manifold.Resolvable = false; // since that, it is not necessary to count collision points

	if (pointA1.x == pointB1.x && pointA2.x == pointB2.x) // no intersection (both are vertical)
	{
		manifold.HasCollision = false;
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
						manifold.HasCollision = true;
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
						manifold.HasCollision = true;
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
				manifold.HasCollision = false;
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
								manifold.HasCollision = true;
							}
						}
					}
				}
			}
		}
	}

	return manifold;
}

Basic::CollisionManifold Basic::CollisionDetection::FindPolygonPolygonCollisionPoints(
	const PolygonCollider* polygonA, const Transform& polygonATransform,
	const PolygonCollider* polygonB, const Transform& polygonBTransform)
{
	using Maths::Dot;
	using Maths::NormalizeVector;

	CollisionManifold manifold;

	// convex collision detection (without resolving it)
	if (!polygonA->IsConvex())
	{
		manifold.HasCollision = false;
		manifold.Resolvable = false;

		auto& triangles = polygonA->GlobalTriangles(polygonATransform);
		auto& vertices = polygonB->GlobalVertices(polygonBTransform);
		for (auto& triangle : triangles)
		{
			if (SAT::Algorithm(vertices, triangle))
			{
				manifold.HasCollision = true;
				break;
			}
		}

		return manifold;
	}
	else if (!polygonB->IsConvex())
	{
		manifold.HasCollision = false;
		manifold.Resolvable = false;

		auto& triangles = polygonB->GlobalTriangles(polygonBTransform);
		auto& vertices = polygonA->GlobalVertices(polygonATransform);
		for (auto& triangle : triangles)
		{
			if (SAT::Algorithm(vertices, triangle))
			{
				manifold.HasCollision = true;
				break;
			}
		}

		return manifold;
	}
	else if(!polygonA->IsConvex() && !polygonB->IsConvex()) // both colliders aren't convex
	{
		manifold.HasCollision = false;
		manifold.Resolvable = false;

		auto& trianglesA = polygonA->GlobalTriangles(polygonATransform);
		auto& trianglesB = polygonB->GlobalTriangles(polygonBTransform);

		for (auto& triangleA : trianglesA)
		{
			for (auto& triangleB : trianglesB)
			{
				if (SAT::Algorithm(triangleA, triangleB))
				{
					manifold.HasCollision = true;
					break;
				}
			}
		}

		return manifold;
	}

	// convex vs convex collision detection, without resolving
	// if one of the colliders is described as not to solve, use SAT Algorithm
	if (!polygonA->Solve || !polygonA->Solve)
	{
		/*collPoints.HasCollision = SAT::Algorithm(&polygon1->GlobalVertices(polygon1Transform),
			&polygon2->GlobalVertices(polygon2Transform));*/

		manifold.HasCollision = GJK::Algorithm(polygonA, polygonATransform,
			polygonB, polygonBTransform);

		manifold.Resolvable = false;

		return manifold;
	}

	// convex vs convex collision detection, with resolving
	manifold = GJK::AlgorithmWithEPA(polygonA, polygonATransform,
		polygonB, polygonBTransform);

	// if collision didn't occur return manifold
	if (!manifold.HasCollision)
		return manifold;

	// do not count collision points if there is no depth of collision
	// when there is no depth, objects are touching but not colliding
	if (manifold.Depth == 0.0f)
		return manifold;

	// in other case start clipping algorithm in order to find contact points
	
	// 1. Get the best walls of both polygons:
	
	// it is worth noting that manifold.Normal is in direction in which
	// object A should be moved to fix collision

	ClippingAlgo::CPEdge ref = polygonA->GetTheBestClippingEdge(polygonATransform, -manifold.Normal);
	ClippingAlgo::CPEdge inc = polygonB->GetTheBestClippingEdge(polygonBTransform, manifold.Normal);

	// 2. Finding ref and inc edges:

	// by default body A is ref and B is inc
	// if it's not the case, raise the flag below
	bool refAndIncAreFlipped = false;

	if (std::abs(Dot(ref.B - ref.A, manifold.Normal)) > std::abs(Dot(inc.B - inc.A, manifold.Normal)))
	{
		// if vector of first edge is more perpendicular to normal then second's edge
		// swap ref and inc(now body A is inc and B is ref)
		std::swap(ref, inc);
		refAndIncAreFlipped = true;
	}

	// 3. Clipping inc edge:

	// initialize contact points
	ContactPoints contactPoints = { inc.A, inc.B };

	ClippingAlgo::Clip(ref.A, ref.B, inc.A, inc.B, contactPoints);
	ClippingAlgo::Clip(ref.B, ref.A, contactPoints[0], contactPoints[1], contactPoints);

	// 4. Final Clipping:

	// check if points are on the right side of the reference edge
	if (Dot(contactPoints[0] - ref.A, ref.Normal) > 0.0f)
	{
		// remove first contact point if it's not on the right side of the edge
		contactPoints = { contactPoints[1] };
	}
	if (Dot(contactPoints[1] - ref.A, ref.Normal) > 0.0f)
	{
		// remove second point if it's not on the right side of the edge
		if (contactPoints.Size() == 2)
		{
			contactPoints = { contactPoints[0] };
		}
		else
		{
			contactPoints = { };
		}
	}

	// if neither of points has been removed, check if they have the same
	// distance from the reference edge
	if (contactPoints.Size() == 2)
	{
		float distance1 = Dot(ref.A - contactPoints[0], ref.Normal);
		float distance2 = Dot(ref.A - contactPoints[1], ref.Normal);

		// if distance1 isn't similar to distance2 remove one point
		if (!(distance1 < distance2 + DEPTH_TOLERANCE
			&& distance1 > distance2 - DEPTH_TOLERANCE))
		{
			if (distance1 > distance2)
			{
				contactPoints = { contactPoints[0] };
			}
			else if (distance2 > distance1)
			{
				contactPoints = { contactPoints[1] };
			}
		}
	}

	// 5. Fix contact points position:

	if (refAndIncAreFlipped)
	{
		// polygonB has ref edge
		if (polygonB->Movable)
		{
			// if polygonB is movable that means
			// that we have to fix contact points
			// since polygonB will change it's position
			// after position solving

			sf::Vector2f fixer = manifold.Depth * manifold.Normal;

			if (polygonA->Movable)
				fixer *= 0.5f;

			for (int i = 0; i < contactPoints.Size(); i++)
				contactPoints[i] += fixer;
		}
	}
	else
	{
		// polygonA has ref edge
		if (polygonA->Movable)
		{
			// if polygonA is movable that means
			// that we have to fix contact points
			// since polygonA will change it's position
			// after position solving

			sf::Vector2f fixer = -manifold.Depth * manifold.Normal;

			if (polygonB->Movable)
				fixer *= 0.5f;

			for (int i = 0; i < contactPoints.Size(); i++)
				contactPoints[i] += fixer;
		}
	}

	// assign counted contactPoints to manifold
	manifold.Points = contactPoints;

	if (contactPoints.Size() == 2)
	{
		CircleShape circle(2.0f);
		circle.setFillColor(sf::Color(255.0f, 0.0f, 255.0f, 0.8f * 255.0f));
		circle.setOrigin(2.0f, 2.0f);
		circle.setPosition(contactPoints[0]);

		CircleShape circle2(2.0f);
		circle2.setFillColor(sf::Color(255.0f, 0.0f, 255.0f, 0.8f * 255.0f));
		circle2.setOrigin(2.0f, 2.0f);
		circle2.setPosition(contactPoints[1]);

		VisualGizmos::DrawOnce(circle);
		VisualGizmos::DrawOnce(circle2);
	}
	else
	{
		CircleShape circle(2.0f);
		circle.setFillColor(sf::Color(255.0f, 0.0f, 255.0f, 0.8f * 255.0f));
		circle.setOrigin(2.0f, 2.0f);
		circle.setPosition(contactPoints[0]);
		
		VisualGizmos::DrawOnce(circle);
	}

	return manifold;
}

Basic::CollisionManifold Basic::CollisionDetection::FindPolygonCircleCollisionPoints(
	const PolygonCollider* polygonA, const Transform& polygonATransform,
	const CircleCollider* circleB, const Transform& circleBTransform)
{
	CollisionManifold manifold;

	// concave polygon vs circle, whitout resolution
	if (!polygonA->IsConvex())
	{
		manifold.HasCollision = false;
		manifold.Resolvable = false;

		return manifold;
	}


	// collision detection without resolution
	// if one of the colliders is described as not to solve
	if (!polygonA->Solve || !circleB->Solve)
	{
		/*manifold.HasCollision = SAT::Algorithm(&polygon1->GlobalVertices(polygon1Transform),
			&polygon2->GlobalVertices(polygon2Transform));*/

		manifold.HasCollision = GJK::Algorithm(polygonA, polygonATransform,
			circleB, circleBTransform);

		manifold.Resolvable = false;

		return manifold;
	}

	// collision detection, with resolution
	manifold = GJK::AlgorithmWithEPA(polygonA, polygonATransform,
		circleB, circleBTransform);

	if (!manifold.HasCollision)
		return manifold;

	if (manifold.Depth == 0.0f)
		return manifold;

	// find contact point
	ContactPoints contactPoints =
	{ circleB->GetGlobalCenterOfGravity(circleBTransform) + circleB->Radius() * manifold.Normal };

	// fix contact point
	if (circleB->Movable)
	{
		sf::Vector2f fixer = -manifold.Depth * manifold.Normal;

		if (polygonA->Movable)
			fixer *= 0.5f;

		contactPoints[0] += fixer;

		CircleShape circle(2.0f);
		circle.setFillColor(sf::Color(255.0f, 0.0f, 255.0f, 0.8f * 255.0f));
		circle.setOrigin(2.0f, 2.0f);
		circle.setPosition(contactPoints[0]);

		VisualGizmos::DrawOnce(circle);
	}

	manifold.Points = contactPoints;

	return manifold;
}

Basic::CollisionManifold Basic::CollisionDetection::FindCirclePolygonCollisionPoints(
	const CircleCollider* circleA, const Transform& circleATransform,
	const PolygonCollider* polygonB, const Transform& polygonBTransform)
{
	CollisionManifold manifold = FindPolygonCircleCollisionPoints(
		polygonB, polygonBTransform, 
		circleA, circleATransform);

	std::swap(manifold.A, manifold.B);
	manifold.Normal = -manifold.Normal;
	
	return manifold;
}

Basic::CollisionManifold Basic::CollisionDetection::FindPolygonPlaneCollisionPoints(
	const PolygonCollider* polygonA, const Transform& polygonATransform,
	const PlaneCollider* planeB, const Transform& planeBTransform)
{
	using Maths::Dot;

	CollisionManifold manifold;

	// concave polygon vs circle collision detection without resolution
	if (!polygonA->IsConvex())
	{
		manifold.HasCollision = false;
		manifold.Resolvable = false;

		return manifold;
	}


	// collision detection without resolution
	// if one of the colliders is described as not to solve
	if (!polygonA->Solve || !planeB->Solve)
	{
		/*manifold.HasCollision = SAT::Algorithm(&polygon1->GlobalVertices(polygon1Transform),
			&polygon2->GlobalVertices(polygon2Transform));*/

		manifold.HasCollision = GJK::Algorithm(polygonA, polygonATransform,
			planeB, planeBTransform);

		manifold.Resolvable = false;

		return manifold;
	}

	// collision detection, with resolution
	manifold = GJK::AlgorithmWithEPA(polygonA, polygonATransform,
		planeB, planeBTransform);

	// if collision didn't occur return manifold
	if (!manifold.HasCollision)
		return manifold;

	// do not count collision points if there is no depth of collision
	// when there is no depth, objects are touching but not colliding
	if (manifold.Depth == 0.0f)
		return manifold;

	// in other case start clipping algorithm in order to find contact points

	// 1. Get the best walls of both polygons:

	// it is worth noting that manifold.Normal is in direction in which
	// object A should be moved to fix collision

	ClippingAlgo::CPEdge ref = polygonA->GetTheBestClippingEdge(polygonATransform, -manifold.Normal);

	// count plane edge
	ClippingAlgo::CPEdge inc;
	inc.A = planeB->GetGlobalAPoint(planeBTransform);
	inc.B = planeB->GetGlobalBPoint(planeBTransform);
	inc.Distance = Maths::Distance(inc.A, inc.B);
	inc.FurthestPoint = planeB->GetGlobalAPoint(planeBTransform);

	sf::Vector2f tempVec = inc.B - inc.A;

	inc.Normal = sf::Vector2f(-tempVec.y, tempVec.x);

	if (Dot(polygonA->GetGlobalCenterOfGravity(polygonATransform) - inc.A, inc.Normal) < 0.0f)
	{
		inc.Normal = -inc.Normal;
	}

	// 2. Finding ref and inc edges:

	// by default body A is ref and B is inc
	// if it's not the case, raise the flag below
	bool refAndIncAreFlipped = false;

	if (std::abs(Dot(ref.B - ref.A, manifold.Normal)) > std::abs(Dot(inc.B - inc.A, manifold.Normal)))
	{
		// if vector of first edge is more perpendicular to normal then second's edge
		// swap ref and inc(now body A is inc and B is ref)
		std::swap(ref, inc);
		refAndIncAreFlipped = true;
	}

	// 3. Clipping inc edge:

	// initialize contact points
	ContactPoints contactPoints = { inc.A, inc.B };

	ClippingAlgo::Clip(ref.A, ref.B, inc.A, inc.B, contactPoints);
	ClippingAlgo::Clip(ref.B, ref.A, contactPoints[0], contactPoints[1], contactPoints);

	// 4. Final Clipping:

	// check if points are on the right side of the reference edge
	if (Dot(contactPoints[0] - ref.A, ref.Normal) > 0.0f)
	{
		// remove first contact point if it's not on the right side of the edge
		contactPoints = { contactPoints[1] };
	}
	if (Dot(contactPoints[1] - ref.A, ref.Normal) > 0.0f)
	{
		// remove second point if it's not on the right side of the edge
		if (contactPoints.Size() == 2)
		{
			contactPoints = { contactPoints[0] };
		}
		else
		{
			contactPoints = { };
		}
	}

	// if neither of points has been removed, check if they have the same
	// distance from the reference edge
	if (contactPoints.Size() == 2)
	{
		float distance1 = Dot(ref.A - contactPoints[0], ref.Normal);
		float distance2 = Dot(ref.A - contactPoints[1], ref.Normal);

		// if distance1 isn't similar to distance2 remove one point
		if (!(distance1 < distance2 + DEPTH_TOLERANCE
			&& distance1 > distance2 - DEPTH_TOLERANCE))
		{
			if (distance1 > distance2)
			{
				contactPoints = { contactPoints[0] };
			}
			else if (distance2 > distance1)
			{
				contactPoints = { contactPoints[1] };
			}
		}
	}

	// 5. Fix contact points position:

	if (refAndIncAreFlipped)
	{
		// polygonB has ref edge
		if (planeB->Movable)
		{
			// if polygonB is movable that means
			// that we have to fix contact points
			// since polygonB will change it's position
			// after position solving

			sf::Vector2f fixer = manifold.Depth * manifold.Normal;

			if (polygonA->Movable)
				fixer *= 0.5f;

			for (int i = 0; i < contactPoints.Size(); i++)
				contactPoints[i] += fixer;
		}
	}
	else
	{
		// polygonA has ref edge
		if (polygonA->Movable)
		{
			// if polygonA is movable that means
			// that we have to fix contact points
			// since polygonA will change it's position
			// after position solving

			sf::Vector2f fixer = -manifold.Depth * manifold.Normal;

			if (planeB->Movable)
				fixer *= 0.5f;

			for (int i = 0; i < contactPoints.Size(); i++)
				contactPoints[i] += fixer;
		}
	}

	// assign counted contactPoints to manifold
	manifold.Points = contactPoints;

	if (contactPoints.Size() == 2)
	{
		CircleShape circle(2.0f);
		circle.setFillColor(sf::Color(255.0f, 0.0f, 255.0f, 0.8f * 255.0f));
		circle.setOrigin(2.0f, 2.0f);
		circle.setPosition(contactPoints[0]);

		CircleShape circle2(2.0f);
		circle2.setFillColor(sf::Color(255.0f, 0.0f, 255.0f, 0.8f * 255.0f));
		circle2.setOrigin(2.0f, 2.0f);
		circle2.setPosition(contactPoints[1]);

		VisualGizmos::DrawOnce(circle);
		VisualGizmos::DrawOnce(circle2);
	}
	else
	{
		CircleShape circle(2.0f);
		circle.setFillColor(sf::Color(255.0f, 0.0f, 255.0f, 0.8f * 255.0f));
		circle.setOrigin(2.0f, 2.0f);
		circle.setPosition(contactPoints[0]);

		VisualGizmos::DrawOnce(circle);
	}

	return manifold;
}

Basic::CollisionManifold Basic::CollisionDetection::FindPlanePolygonCollisionPoints(
	const PlaneCollider* planeA, const Transform& planeATransform,
	const PolygonCollider* polygonB, const Transform& polygonBTransform)
{
	CollisionManifold manifold = FindPolygonPlaneCollisionPoints(
		polygonB, polygonBTransform,
		planeA, planeATransform);

	std::swap(manifold.A, manifold.B);
	manifold.Normal = -manifold.Normal;

	return manifold;
}

sf::Vector2f Basic::ClippingAlgo::GetIntersectionPointOfTwoLines(sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D)
{
	float num1 = (A.x * B.y - A.y * B.x) * (C.x - D.x) -
		(A.x - B.x) * (C.x * D.y - C.y * D.x);

	float den1 = (A.x - B.x) * (C.y - D.y) - (A.y - B.y) * (C.x - D.x);

	float num2 = (A.x * B.y - A.y * B.x) * (C.y - D.y) -
		(A.y - B.y) * (C.x * D.y - C.y * D.x);

	float den2 = (A.x - B.x) * (C.y - D.y) - (A.y - B.y) * (C.x - D.x);

	return sf::Vector2f(num1 / den1, num2 / den2);
}

void Basic::ClippingAlgo::Clip(const sf::Vector2f& refA, const sf::Vector2f& refB, const sf::Vector2f& incA, const sf::Vector2f& incB, ContactPoints& points)
{
	using Maths::Dot;

	// define border line by two points
	sf::Vector2f rAB = refB - refA;
	sf::Vector2f rABPerp = sf::Vector2f(-rAB.y, rAB.x);
	sf::Vector2f refC = refA + rABPerp;

	// border line is defined by refA and refC
	// incident line is defined by incA and incB

	// check if intersection of line refA-refC and segment incA-incB exists

	if (Dot(rAB, points[0] - refA) < 0.0f)
	{
		// incA has to be moved

		points[0] = GetIntersectionPointOfTwoLines(refA, refC, incA, incB);
	}
	else if (Dot(rAB, points[1] - refA) < 0.0f)
	{
		// incB has to be moved

		points[1] = GetIntersectionPointOfTwoLines(refA, refC, incA, incB);
	}
}


