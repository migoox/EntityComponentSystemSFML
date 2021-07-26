#pragma once
#include <math.h>

#include "../PhysicalComponents/ColliderItem.h"

namespace Basic {
	namespace MathFunctions {
		float DistanceFromLine(sf::Vector2f point, sf::Vector2f lineA, sf::Vector2f lineB);
		float Distance(sf::Vector2f pointA, sf::Vector2f pointB);
		float VectorDistance(sf::Vector2f vector);
		sf::Vector2f NormalizeVector(sf::Vector2f vector);
		float IsUnderLine(sf::Vector2f point, sf::Vector2f lineA, sf::Vector2f lineB);
		
	}
	namespace CollisionDetection {
		CollisionPoints FindCircleCircleCollisionPoints(
			const CircleCollider* circle1, const Transform& transform1,
			const CircleCollider* circle2, const Transform& transform2);

		CollisionPoints FindCirclePlaneCollisionPoints(
			const CircleCollider* circle, const Transform& circleTransform,
			const PlaneCollider* plane, const Transform& planeTransform);

		CollisionPoints FindPlanePlaneCollisionPoints(
			const PlaneCollider* plane1, const Transform& plane1Transform,
			const PlaneCollider* plane2, const Transform& plane2Transform);


	}
}