#pragma once
#include <SFML/Graphics.hpp>
#include "ColliderItem.h"
#include "../CollisionDetectionAlgorithms.h"

namespace Basic {
	class RectangleCollider : public ColliderItem
	{
	private:
		sf::Vector2f m_ColliderDisplacement;
		float m_ColliderRotation = 0.0f;

		sf::Vector2f m_Size = sf::Vector2f(0.0f, 0.0f);

		std::vector<sf::Vector2f> m_Vertices;
		mutable std::vector<sf::Vector2f> m_GlobalVertices;
	private:
		void Init();

		sf::Vector2f TranslateRelativePointToGlobal(sf::Vector2f point, const Transform& trans) const;
	
		void UpdateGlobalVertices(const Transform& trans) const;

	public:
		RectangleCollider() {}
		RectangleCollider(sf::Vector2f size) : m_Size(size) { Init(); }
		RectangleCollider(sf::Vector2f size, sf::Vector2f displacement) : m_Size(size), m_ColliderDisplacement(displacement) { Init();  }

		void MoveCollider(sf::Vector2f displacement) override;

		void RotateCollider(float angle) override;

		sf::Vector2f GetGlobalCenterOfGravity(const Transform& trans) const override;

		float GetMomentOfInertia(const RigidBody& rb) const override;

		void DrawOnceOnVisualGizmos(const Transform& trans) const override;

		const std::vector<sf::Vector2f>& GlobalVertices(const Transform& trans) const;

		CollisionPoints TestCollision(
			const Transform& transform,
			const ColliderItem* collider,
			const Transform& colliderTransform) const
		{
			return collider->TestCollision(colliderTransform, this, transform);
		}

		CollisionPoints TestCollision(
			const Transform& transform,
			const CircleCollider* circleCollider,
			const Transform& circleTransform) const
		{
			return CollisionDetection::FindRectangleCircleCollisionPoints(this, transform, circleCollider, circleTransform);
		}

		CollisionPoints TestCollision(
			const Transform& transform,
			const PlaneCollider* planeCollider,
			const Transform& planeTransform) const
		{
			return CollisionDetection::FindRectanglePlaneCollisionPoints(this, transform, planeCollider, planeTransform);
		}

		CollisionPoints TestCollision(
			const Transform& transform,
			const PolygonCollider* polygonCollider,
			const Transform& polygonTransform) const
		{
			return CollisionDetection::FindRectanglePolygonCollisionPoints(this, transform, polygonCollider, polygonTransform);
		}

		CollisionPoints TestCollision(
			const Transform& transform,
			const RectangleCollider* rectangleCollider,
			const Transform& rectangleTransform) const
		{
			return CollisionDetection::FindRectangleRectangleCollisionPoints(this, transform, rectangleCollider, rectangleTransform);
		}
	};
}