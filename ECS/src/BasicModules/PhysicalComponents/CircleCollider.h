#pragma once
#include "ColliderItem.h"
#include "../Physics/CollisionDetectionAlgorithms.h"
#include <iostream>
namespace Basic {
	struct CircleCollider : public ColliderItem
	{
		sf::Vector2f Center; // relative to game object's tranform
		float Radius = 0.0f;

		CircleCollider() = default;

		CircleCollider(float radius) : Radius(radius) { }
		CircleCollider(float radius, sf::Vector2f center) : Radius(radius), Center(center) { }

		void Print() override
		{
			std::cout << Radius << std::endl;
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