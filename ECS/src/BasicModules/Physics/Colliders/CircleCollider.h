#pragma once
#include "ColliderItem.h"
#include "../CollisionDetectionAlgorithms.h"

namespace Basic {
	struct CircleCollider : public ColliderItem
	{
		sf::Vector2f Center; // relative to game object's transform
		float Radius = 0.0f;

		CircleCollider() { }

		CircleCollider(float radius) : Radius(radius) { }
		CircleCollider(float radius, sf::Vector2f center) : Radius(radius), Center(center) { }

		sf::Vector2f GetGlobalCenterOfGravity(const Transform& trans) const override
		{
			return Center + trans.getPosition();
		}

		float GetMomentumOfInertia(const RigidBody& rb) const override
		{
			return 0.5f * float(std::pow(Radius, 2)) * rb.Mass;
		}

		void DrawOnceOnVisualGizmos(const Transform& trans) const override
		{
			
		}

		sf::Vector2f GetGlobalCenter(const Transform& trans) const
		{
			return Center + trans.getPosition();
		}

		bool ContainsGlobalPoint(const sf::Vector2f globalPoint, const Transform& trans)
		{
			sf::Vector2f globalCenter = GetGlobalCenter(trans);

			return Radius >= MathFunctions::Distance(globalPoint, globalCenter);
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
			return CollisionDetection::FindCircleCircleCollisionPoints(this, transform, circleCollider, circleTransform);
		}

		CollisionPoints TestCollision(
			const Transform& transform,
			const PlaneCollider* planeCollider,
			const Transform& planeTransform) const override
		{
			return CollisionDetection::FindCirclePlaneCollisionPoints(this, transform, planeCollider, planeTransform);
		}

		CollisionPoints TestCollision(
			const Transform& transform,
			const PolygonCollider* polygonCollider,
			const Transform& polygonTransform) const override
		{
			return CollisionDetection::FindCirclePolygonCollisionPoints(this, transform, polygonCollider, polygonTransform);
		}
	};
}