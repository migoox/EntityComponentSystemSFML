#pragma once
#include <vector>
#include <array>
#include <list>
#include <memory>
#include <iostream>

#include "ColliderItem.h"
#include "../CollisionDetectionAlgorithms.h"
#include <cassert>

namespace Basic {
	namespace PolygonHelpers{
		template<typename T>
		T& GetItem(std::vector<T>& vector, int index)
		{
			int n = vector.size();
			if (index >= n)
				return vector[index % n];
			else if (index < 0)
				return vector[index % n + n];
			else
				return vector[index];
		}

		template<typename T>
		const T& GetItem(const std::vector<T>& vector, int index)
		{
			int n = vector.size();
			if (index >= n)
				return vector[index % n];
			else if (index < 0)
				return vector[index % n + n];
			else
				return vector[index];
		}

		int FixIndex(int index, int size);

		bool PointInTriangle(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& p);

		bool LinesIntersect(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& d);
	}

	class PolygonCollider : public ColliderItem
	{
	private:
		std::vector<std::array<sf::Vector2f, 3>> m_Triangles;
		mutable std::vector<std::array<sf::Vector2f, 3>> m_GlobalTriangles;

	protected:
		sf::Vector2f m_CenterOfGravity;

		sf::Vector2f m_ColliderDisplacement;
		float m_ColliderRotation;

		int m_FurthestPointFromCentroidIndex;

		bool m_Correct;
		bool m_Convex;

		// collider's center of gravity should be placed on (0, 0) point
		// fixer is a vector which moves center of gravity to (0, 0), it is required in polygon
		sf::Vector2f m_Fixer;

		std::vector<sf::Vector2f> m_Vertices;
		mutable std::vector<sf::Vector2f> m_GlobalVertices;

	protected:
		void Init();

		void UpdatePolygon();

		void UpdateGlobalVertices(const Transform& trans) const;

		void UpdateGlobalTriangles(const Transform& trans) const;

		bool IsSimple(const std::vector<sf::Vector2f>& vertices);

		bool IsPolygonConvex(const std::vector<sf::Vector2f>& vertices);

		bool ContainsColinearEdges(const std::vector<sf::Vector2f>& vertices);

		void Triangulate(const std::vector<sf::Vector2f>& vertices, std::vector<std::array<sf::Vector2f, 3>>& triangles);

		sf::Vector2f FindCenterOfGravity(const std::vector<sf::Vector2f>& vertices) const;

		sf::Vector2f TranslateRelativePointToGlobal(sf::Vector2f point, const Transform& trans) const;

		int FindFurthestPointFromCentroidIndex();

	public:
		PolygonCollider();

		PolygonCollider(std::initializer_list<sf::Vector2f> list);

		PolygonCollider(const std::vector<sf::Vector2f>& vector);

		PolygonCollider(std::initializer_list<sf::Vector2f> list, sf::Vector2f displacementOfCenter);

		void Clear();

		// relative poisition
		void SetVertex(size_t index, sf::Vector2f vertex);

		// relative position
		void AddVertex(sf::Vector2f vertex);

		// moves collider relatively to game object's transform
		void MoveCollider(sf::Vector2f displacement) override;

		void RotateCollider(float angle) override;

		sf::Vector2f GetGlobalCenterOfGravity(const Transform& trans) const override;

		float GetMomentOfInertia(const RigidBody& rb) const override;

		AABB GetGlobalAABB(const Transform& trans) const override;

		void DrawOnceOnVisualGizmos(const Transform& trans) const override;

		void DrawAABBOnceOnVisualGizmos(const Transform& trans) const override;

		const std::vector<sf::Vector2f>& GlobalVertices(const Transform& trans) const;

		const std::vector<std::array<sf::Vector2f, 3>>& GlobalTriangles(const Transform& trans) const;

		bool IsCorrect() const { return m_Correct; }

		bool IsConvex() const { return m_Convex; }
		
		sf::Vector2f FindFurthestPointInDirection(const Transform& transform, sf::Vector2f direction) const override;

		int FindFurthestPointInDirectionIndex(const Transform& transform, sf::Vector2f direction) const;

		float GetLongestDistanceFromCentroid() const;

		sf::Vector2f GetGlobalPoint(const Transform& transform, int index) const;

		bool ContainsPoint(sf::Vector2f point);

		ClippingAlgo::CPEdge GetTheBestClippingEdge(const Transform& transform, sf::Vector2f normal) const;

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
			return CollisionDetection::FindCirclePolygonCollisionPoints(circleCollider, circleTransform, this, transform);
		}

		CollisionManifold TestCollision(
			const Transform& transform,
			const PlaneCollider* planeCollider,
			const Transform& planeTransform) const override
		{
			return CollisionDetection::FindPlanePolygonCollisionPoints(planeCollider, planeTransform, this, transform);
		}

		CollisionManifold TestCollision(
			const Transform& transform,
			const PolygonCollider* polygonCollider,
			const Transform& polygonTransform) const override
		{
			return CollisionDetection::FindPolygonPolygonCollisionPoints(polygonCollider, polygonTransform, this, transform);
		}
	};
}