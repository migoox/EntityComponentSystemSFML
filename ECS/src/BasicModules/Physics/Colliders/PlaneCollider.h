#pragma once
#include "ColliderItem.h"
#include "../CollisionDetectionAlgorithms.h"

namespace Basic {
	class PlaneCollider : public ColliderItem
	{
	private:
		sf::Vector2f m_ColliderDisplacement;
		float m_ColliderRotation = 0.0f;

		float m_Distance = 0.0f;
	private:

		sf::Vector2f TranslateRelativePointToGlobal(sf::Vector2f point, const Transform& trans) const;

	public:
		PlaneCollider() { }

		PlaneCollider(float distance) : m_Distance(distance) { }

		PlaneCollider(float distance, sf::Vector2f displacement) : m_Distance(distance), m_ColliderDisplacement(displacement) { }

		float Distance() const;

		sf::Vector2f GetGlobalCenterOfGravity(const Transform& trans) const override;

		float GetMomentOfInertia(const RigidBody& rb) const override;

		AABB GetGlobalAABB(const Transform& trans) const override;

		void DrawOnceOnVisualGizmos(const Transform& trans) const override;

		void DrawAABBOnceOnVisualGizmos(const Transform& trans) const override;

		void MoveCollider(sf::Vector2f displacement) override;

		void RotateCollider(float angle) override;

		sf::Vector2f GetGlobalAPoint(const Transform& trans) const;

		sf::Vector2f GetGlobalBPoint(const Transform& trans) const;

		sf::Vector2f FindFurthestPointInDirection(const Transform& transform, sf::Vector2f direction) const override;

		// collision tests
		CollisionManifold TestCollision(
			const Transform& transform,
			const ColliderItem* collider,
			const Transform& colliderTransform) const override
		{
			return collider->TestCollision(colliderTransform, this, transform);
		}

		CollisionManifold TestCollision(
			const Transform& transform,
			const CircleCollider* circleCollider,
			const Transform& circleTransform) const override
		{
			return CollisionDetection::FindCirclePlaneCollisionPoints(circleCollider, circleTransform, this, transform);
		}

		CollisionManifold TestCollision(
			const Transform& transform,
			const PlaneCollider* planeCollider,
			const Transform& planeTransform) const override
		{
			return CollisionDetection::FindPlanePlaneCollisionPoints(planeCollider, planeTransform, this, transform);
		}

		CollisionManifold TestCollision(
			const Transform& transform,
			const PolygonCollider* polygonCollider,
			const Transform& polygonTransform) const override
		{
			return CollisionDetection::FindPolygonPlaneCollisionPoints(polygonCollider, polygonTransform, this, transform);
		}
	};
}