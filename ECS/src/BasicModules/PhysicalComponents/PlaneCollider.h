#pragma once
#include "ColliderItem.h"
#include "../Physics/CollisionDetectionAlgorithms.h"

namespace Basic {
	struct PlaneCollider : public ColliderItem
	{
		sf::Vector2f Plane;
		float Distance;

		PlaneCollider() = default;

		PlaneCollider(sf::Vector2f point, float distance) : Plane(point), Distance(distance) { }
		PlaneCollider(float distance) : Plane(sf::Vector2f(0.0f, 0.0f)), Distance(distance) { }

		CollisionPoints TestCollision(
			const Transform& transform,
			const ColliderItem* collider,
			const Transform& colliderTransform) const override
		{
			return collider->TestCollision(colliderTransform, this, transform);
		}

		CollisionPoints TestCollision(
			const Transform& transform,
			const CircleCollider* circleCollider,
			const Transform& circleTransform) const override
		{
			return CollisionDetection::FindCirclePlaneCollisionPoints(
				circleCollider, circleTransform, this, transform);
		}

		CollisionPoints TestCollision(
			const Transform& transform,
			const PlaneCollider* planeCollider,
			const Transform& planeTransform) const override
		{
			return CollisionPoints();
		}
	};
}