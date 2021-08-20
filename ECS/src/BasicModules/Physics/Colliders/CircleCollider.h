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

		void MoveCollider(sf::Vector2f displacement) override
		{
			m_ColliderDisplacement += displacement;
		}

		void DrawOnceOnVisualGizmos(const Transform& trans) const override
		{
			CircleShape shape(m_Radius);
			shape.setFillColor(sf::Color::Blue);
			shape.setOrigin(m_Radius, m_Radius);
			shape.setPosition(GetGlobalCenterOfGravity(trans));

			CircleShape gCenter(2.0f);
			gCenter.setFillColor(sf::Color::Red);
			gCenter.setOrigin(1.0f, 1.0f);
			gCenter.setPosition(GetGlobalCenterOfGravity(trans));

			Basic::VisualGizmos::DrawOnce(shape);
			Basic::VisualGizmos::DrawOnce(gCenter);
		}

		bool ContainsGlobalPoint(const sf::Vector2f globalPoint, const Transform& trans)
		{
			sf::Vector2f globalCenter = GetGlobalCenterOfGravity(trans);

			return m_Radius >= MathFunctions::Distance(globalPoint, globalCenter);
		}

		sf::Vector2f FindFurthestPointInDirection(const Transform& transform, sf::Vector2f direction) const override
		{
			return GetGlobalCenterOfGravity(transform) + m_Radius * MathFunctions::NormalizeVector(direction);
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