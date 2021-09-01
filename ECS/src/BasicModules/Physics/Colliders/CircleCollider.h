#pragma once
#include "ColliderItem.h"
#include "../CollisionDetectionAlgorithms.h"

namespace Basic {
	class CircleCollider : public ColliderItem
	{
	private:
		sf::Vector2f m_ColliderDisplacement;

		float m_Radius = 0.0f;

	public:
		CircleCollider() { }

		CircleCollider(float radius) : m_Radius(radius) { }
		CircleCollider(float radius, sf::Vector2f displacement) : m_Radius(radius), m_ColliderDisplacement(displacement) { }

		float Radius() const
		{
			return m_Radius;
		}

		sf::Vector2f GetGlobalCenterOfGravity(const Transform& trans) const override
		{
			sf::Vector2f scale = trans.getScale();
			float angle = trans.getRotation() * 3.141592653f / 180.0f;

			sf::Vector2f global = m_ColliderDisplacement;

			global.x *= scale.x;
			global.y *= scale.y;

			global = sf::Vector2f(global.x * std::cos(angle) - global.y * std::sin(angle),
				global.x * std::sin(angle) + global.y * std::cos(angle));
			global += trans.getPosition();

			return global;
		}

		float GetMomentOfInertia(const RigidBody& rb) const override
		{
			return 0.5f * float(std::pow(m_Radius, 2)) * rb.Mass;
		}

		AABB GetGlobalAABB(const Transform& trans) const override
		{
			sf::Vector2f globalCenter = GetGlobalCenterOfGravity(trans);
			sf::Vector2f helper = sf::Vector2f(m_Radius, m_Radius);

			return { globalCenter - helper, globalCenter + helper };
		}

		void MoveCollider(sf::Vector2f displacement) override
		{
			m_ColliderDisplacement += displacement;
		}

		void DrawOnceOnVisualGizmos(const Transform& trans) const override
		{
			/*CircleShape shape(m_Radius);
			shape.setFillColor(sf::Color(0.0f, 255.0f, 0.0f, 0.8f * 255.0f));
			shape.setOrigin(m_Radius, m_Radius);
			shape.setPosition(GetGlobalCenterOfGravity(trans));

			CircleShape gCenter(2.0f);
			gCenter.setFillColor(sf::Color(255.0f, 0.0f, 0.0f, 0.8f * 255.0f));
			gCenter.setOrigin(1.0f, 1.0f);
			gCenter.setPosition(GetGlobalCenterOfGravity(trans));

			Basic::VisualGizmos::DrawOnce(shape);
			Basic::VisualGizmos::DrawOnce(gCenter);*/
		}

		void DrawAABBOnceOnVisualGizmos(const Transform& trans) const override
		{
			/*AABB aabb = GetGlobalAABB(trans);

			Basic::RectangleShape rect;
			rect.setSize(sf::Vector2f(aabb.maxPoint - aabb.minPoint));
			rect.setPosition(aabb.minPoint);
			rect.setFillColor(sf::Color(0.0f, 150.0f, 50.0f, 0.4f * 255.0f));

			Basic::VisualGizmos::DrawOnce(rect);*/
		}

		bool ContainsGlobalPoint(const sf::Vector2f globalPoint, const Transform& trans)
		{
			sf::Vector2f globalCenter = GetGlobalCenterOfGravity(trans);

			return m_Radius >= Maths::Distance(globalPoint, globalCenter);
		}

		sf::Vector2f FindFurthestPointInDirection(const Transform& transform, sf::Vector2f direction) const override
		{
			return GetGlobalCenterOfGravity(transform) + m_Radius * Maths::NormalizeVector(direction);
		}

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
			return CollisionDetection::FindCircleCircleCollisionPoints(circleCollider, circleTransform, this, transform);
		}

		CollisionManifold TestCollision(
			const Transform& transform,
			const PlaneCollider* planeCollider,
			const Transform& planeTransform) const override
		{
			return CollisionDetection::FindPlaneCircleCollisionPoints(planeCollider, planeTransform, this, transform);
		}

		CollisionManifold TestCollision(
			const Transform& transform,
			const PolygonCollider* polygonCollider,
			const Transform& polygonTransform) const override
		{
			return CollisionDetection::FindPolygonCircleCollisionPoints(polygonCollider, polygonTransform, this, transform);
		}
	};
}