#pragma once
#include "Collider.h"
#include "../Physics/CollisionDetectionAlgorithms.h"

namespace Basic {
	struct CircleCollider : public Collider
	{
		sf::Vector2f Center; // relative to game object's tranform
		float Radius = 0.0f;

		CircleCollider() = default;

		CircleCollider(float radius) : Radius(radius) { }
		CircleCollider(float radius, sf::Vector2f center) : Radius(radius), Center(center) { }

		CollisionPoints TestCollision(
			const Transform& transform,
			const CircleCollider& circleCollider,
			const Transform& circleTransform) const override
		{
			return CollisionDetection::FindCircleCircleCollisionPoints(
				*this, transform, 
				circleCollider, circleTransform);
		}

		CollisionPoints TestCollision(
			const Transform& transform,
			const PlaneCollider& planeCollider,
			const Transform& planeTransform) const override
		{
			return CollisionDetection::FindCirclePlaneCollisionPoints(
				*this, transform,
				planeCollider, planeTransform);
		}
	};
}