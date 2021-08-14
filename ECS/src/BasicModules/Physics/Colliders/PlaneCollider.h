#pragma once
#include "ColliderItem.h"
#include "../CollisionDetectionAlgorithms.h"

namespace Basic {
	struct PlaneCollider : public ColliderItem
	{
		sf::Vector2f Plane;
		float Distance = 0.0f;

		PlaneCollider() { }

		PlaneCollider(sf::Vector2f point, float distance) : Plane(point), Distance(distance) { }
		PlaneCollider(float distance) : Plane(sf::Vector2f(0.0f, 0.0f)), Distance(distance) { }

		sf::Vector2f GetGlobalCenterOfGravity(const Transform& trans) const override
		{
			return Plane + trans.getPosition();
		}

		float GetMomentumOfInertia(const RigidBody& rb) const override
		{
			return 1.0f / 12.0f * float(std::pow(Distance, 2)) * rb.Mass;
		}

		sf::Vector2f GetGlobalAPoint(const Transform& trans) const
		{
			float pi = 3.141592653f;

			sf::Vector2f pointA;

			pointA.x = trans.getPosition().x + Plane.x - std::cos(trans.getRotation() * pi / 180.0f) * Distance / 2;
			pointA.y = trans.getPosition().y + Plane.y - std::sin(trans.getRotation() * pi / 180.0f) * Distance / 2;

			return pointA;
		}

		sf::Vector2f GetGlobalBPoint(const Transform& trans) const
		{
			float pi = 3.141592653f;

			sf::Vector2f pointB;

			pointB.x = trans.getPosition().x + Plane.x + std::cos(trans.getRotation() * pi / 180.0f) * Distance / 2;
			pointB.y = trans.getPosition().y + Plane.y + std::sin(trans.getRotation() * pi / 180.0f) * Distance / 2;

			return pointB;
		}

		// collision tests
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
			return CollisionDetection::FindPlaneCircleCollisionPoints(this, transform, circleCollider, circleTransform);
		}

		CollisionPoints TestCollision(
			const Transform& transform,
			const PlaneCollider* planeCollider,
			const Transform& planeTransform) const override
		{
			return CollisionDetection::FindPlanePlaneCollisionPoints(this, transform, planeCollider, planeTransform);
		}

		CollisionPoints TestCollision(
			const Transform& transform,
			const PolygonCollider* convexCollider,
			const Transform& convexTransform) const override
		{
			return CollisionDetection::FindPlanePolygonCollisionPoints(this, transform, convexCollider, convexTransform);
		}
	};
}