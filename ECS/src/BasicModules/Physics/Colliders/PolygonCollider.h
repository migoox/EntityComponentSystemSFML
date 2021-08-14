#pragma once
#include <vector>
#include <list>

#include "ColliderItem.h"
#include "../CollisionDetectionAlgorithms.h"
#include <cassert>

namespace Basic {
	namespace Helpers{
		template<typename T>
		T& GetItem(std::vector<T>& vector, size_t index)
		{
			if (index >= vector.size())
				return vector[index % vector.size()];
			else if (index < 0)
				return vector[index % vector.size() + vector.size()];
			else
				return vector[index];
		}

		bool PointInTriangle(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& p);
	}

	class PolygonCollider : public ColliderItem
	{
	private:
		sf::Vector2f m_CenterOfGravity;
		sf::Vector2f m_CenterDisplacement;

		sf::Vector2f m_Fixer; // usefull for transformation of polygon

		bool m_Correct;
		bool m_Convex;

		std::vector<Triangle> m_Triangles;
		mutable std::vector<Triangle> m_GlobalTriangles;

		std::vector<sf::Vector2f> m_Vertices;
		mutable std::vector<sf::Vector2f> m_GlobalVertices;

	private:
		void Init();

		void UpdatePolygon();

		void UpdateGlobalVertices(const Transform& trans) const;

		void UpdateGlobalTriangles(const Transform& trans) const;

		bool IsSimple(const std::vector<sf::Vector2f>& vertices);

		bool IsPolygonConvex(std::vector<sf::Vector2f>& vertices);

		bool ContainsColinearEdges(const std::vector<sf::Vector2f>& vertices);

		void Triangulate(const std::vector<sf::Vector2f>& vertices, std::vector<Triangle>& triangles);

		sf::Vector2f FindCenterOfGravity(const std::vector<sf::Vector2f>& vertices);

	public:
		PolygonCollider();

		PolygonCollider(std::initializer_list<sf::Vector2f> list);

		PolygonCollider(const std::vector<sf::Vector2f>& vector);

		// relative poisition
		void SetVertex(size_t index, sf::Vector2f vertex);

		// relative position
		void AddVertex(sf::Vector2f vertex);

		// moves collider relatively to game object's transform
		void MoveCollider(sf::Vector2f displacement);

		sf::Vector2f GetGlobalCenterOfGravity(const Transform& trans) const override;

		float GetMomentumOfInertia(const RigidBody& rb) const override;

		const std::vector<sf::Vector2f>& GlobalVertices(const Transform& trans) const;

		const std::vector<Triangle>& GlobalTriangles(const Transform& trans) const;

		bool IsCorrect() const { return m_Correct; }

		bool IsConvex() const { return m_Convex; }

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
			return CollisionDetection::FindPolygonCircleCollisionPoints(
				this, transform, circleCollider, circleTransform);
		}

		CollisionPoints TestCollision(
			const Transform& transform,
			const PlaneCollider* planeCollider,
			const Transform& planeTransform) const override
		{
			return CollisionDetection::FindPolygonPlaneCollisionPoints(this, transform, planeCollider, planeTransform);
		}

		CollisionPoints TestCollision(
			const Transform& transform,
			const PolygonCollider* PolygonCollider,
			const Transform& PolygonTransform) const override
		{
			return CollisionDetection::FindPolygonPolygonCollisionPoints(this, transform, PolygonCollider, PolygonTransform);
		}
	};
}