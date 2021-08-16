#pragma once
#include <math.h>

#include "../Maths/Maths.h"
#include "Colliders/ColliderItem.h"

namespace Basic {
	namespace CollisionDetection {
		CollisionPoints FindCircleCircleCollisionPoints(
			const CircleCollider* circle1, const Transform& transform1,
			const CircleCollider* circle2, const Transform& transform2);

		CollisionPoints FindCirclePlaneCollisionPoints(
			const CircleCollider* circle, const Transform& circleTransform,
			const PlaneCollider* plane, const Transform& planeTransform);

		CollisionPoints FindPlaneCircleCollisionPoints(
			const PlaneCollider* plane, const Transform& planeTransform,
			const CircleCollider* circle, const Transform& circleTransform);

		CollisionPoints FindPlanePlaneCollisionPoints(
			const PlaneCollider* plane1, const Transform& plane1Transform,
			const PlaneCollider* plane2, const Transform& plane2Transform);

		CollisionPoints FindPolygonPolygonCollisionPoints(
			const PolygonCollider* polygon1, const Transform& polygon1Transform,
			const PolygonCollider* polygon2, const Transform& polygon2Transform);

		CollisionPoints FindPolygonCircleCollisionPoints(
			const PolygonCollider* polygon, const Transform& polygonTransform,
			const CircleCollider* circle, const Transform& circleTransform);

		CollisionPoints FindCirclePolygonCollisionPoints(
			const CircleCollider* circle, const Transform& circleTransform,
			const PolygonCollider* polygon, const Transform& polygonTransform);

		CollisionPoints FindPolygonPlaneCollisionPoints(
			const PolygonCollider* polygon, const Transform& polygonTransform,
			const PlaneCollider* plane, const Transform& planeTransform);

		CollisionPoints FindPlanePolygonCollisionPoints(
			const PlaneCollider* plane, const Transform& planeTransform,
			const PolygonCollider* polygon, const Transform& polygonTransform);

		CollisionPoints FindRectanglePolygonCollisionPoints(
			const RectangleCollider* rectangle, const Transform& rectangleTransform,
			const PolygonCollider* polygon, const Transform& polygonTransform);

		CollisionPoints FindPolygonRectangleCollisionPoints(
			const PolygonCollider* polygon, const Transform& polygonTransform,
			const RectangleCollider* rectangle, const Transform& rectangleTransform);

		CollisionPoints FindRectangleCircleCollisionPoints(
			const RectangleCollider* rectangle, const Transform& rectangleTransform,
			const CircleCollider* circle, const Transform& circleTransform);

		CollisionPoints FindCircleRectangleCollisionPoints(
			const CircleCollider* circle, const Transform& circleTransform,
			const RectangleCollider* rectangle, const Transform& rectangleTransform);

		CollisionPoints FindRectanglePlaneCollisionPoints(
			const RectangleCollider* rectangle, const Transform& rectangleTransform,
			const PlaneCollider* plane, const Transform& planeTransform);

		CollisionPoints FindPlaneRectangleCollisionPoints(
			const PlaneCollider* plane, const Transform& planeTransform,
			const RectangleCollider* rectangle, const Transform& rectangleTransform);

		CollisionPoints FindRectangleRectangleCollisionPoints(
			const RectangleCollider* rectangle1, const Transform& rectangle1Transform,
			const RectangleCollider* rectangle2, const Transform& rectangle2Transform);
	}
}