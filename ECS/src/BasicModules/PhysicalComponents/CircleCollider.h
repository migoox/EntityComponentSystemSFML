#pragma once
#include "ColliderItem.h"
#include "../Physics/CollisionDetectionAlgorithms.h"

namespace Basic {
	struct CircleCollider : public ColliderItem
	{
		sf::Vector2f Center; // relative to game object's tranform
		float Radius = 0.0f;

		CircleCollider() { }

		CircleCollider(float radius) : Radius(radius) { }
		CircleCollider(float radius, sf::Vector2f center) : Radius(radius), Center(center) { }

		sf::Vector2f GetGlobalCenter(const Transform& trans) const
		{
			return Center + trans.getPosition();
		}

		bool ContainsGlobalPoint(const sf::Vector2f globalPoint, const Transform& trans)
		{
			sf::Vector2f globalCenter = GetGlobalCenter(trans);

			return Radius >= MathFunctions::Distance(globalPoint, globalCenter);
		}

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
			return CollisionDetection::FindCircleCircleCollisionPoints(
				this, transform, 
				circleCollider, circleTransform);
		}

		CollisionPoints TestCollision(
			const Transform& transform,
			const PlaneCollider* planeCollider,
			const Transform& planeTransform) const override
		{
			return CollisionDetection::FindCirclePlaneCollisionPoints(
				this, transform,
				planeCollider, planeTransform);
		}
	};
}