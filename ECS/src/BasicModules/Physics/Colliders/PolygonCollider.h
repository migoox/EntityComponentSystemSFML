#pragma once
#include <vector>
#include <list>

#include "ColliderItem.h"
#include "../CollisionDetectionAlgorithms.h"

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

	struct Polygon
	{
		std::vector<sf::Vector2f> m_GlobalVertices;
	};

	class PolygonCollider : public ColliderItem
	{
	private:
		sf::Vector2f m_RelativeCenterPosition;
		sf::Vector2f m_GlobalCenterPosition;

		bool m_Correct;

		bool m_Convex;
		std::vector<Polygon> m_Polygons;

		std::vector<sf::Vector2f> m_Vertices;

	private:
		void Init();

		void UpdatePolygon();

		bool IsSimple(const std::vector<sf::Vector2f>& vertices);

		bool IsPolygonConvex(std::vector<sf::Vector2f>& vertices);

		bool ContainsColinearEdges(const std::vector<sf::Vector2f>& vertices);

		// ear clipping algorithm
		bool Triangulate(const std::vector<sf::Vector2f>& vertices, std::vector<Polygon>& triangles);

	public:
		PolygonCollider();

		PolygonCollider(std::initializer_list<sf::Vector2f> list);

		const std::vector<sf::Vector2f>& Vertices() const { return m_Vertices; }
		
		const std::vector<Polygon>& Polygons() const { return m_Polygons; }

		bool IsCorrect() const { return m_Correct; }

		bool IsConvex() const { return m_Convex; }

		void SetCenter(sf::Vector2f center);

		void SetVertex(size_t vertex, sf::Vector2f value);

		sf::Vector2f GetGlobalCenterOfGravity(const Transform& trans) const override { return m_GlobalCenterPosition; };

		float GetMomentumOfInertia(const RigidBody& rb) const override { return 0.0f; };

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