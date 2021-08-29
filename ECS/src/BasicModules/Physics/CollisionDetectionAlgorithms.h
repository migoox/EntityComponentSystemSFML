#pragma once
#include <math.h>
#include <array>
#include <list>

#include "../Maths/Maths.h"
#include "Colliders/ColliderItem.h"

namespace Basic {

	namespace GJK {
		class Simplex
		{
		private:
			std::array<sf::Vector2f, 3> m_Points;

			unsigned int m_Size;

		public:
			Simplex()
				: m_Points({ sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f) }), m_Size(0u)
			{}

			Simplex(std::initializer_list<sf::Vector2f> list)
				: m_Points({ sf::Vector2f(0.f, 0.0f), sf::Vector2f(0.f, 0.0f), sf::Vector2f(0.f, 0.0f) }), m_Size(0u)
			{
				*this = list;
			}

			Simplex& operator=(std::initializer_list<sf::Vector2f> list)
			{
				for (auto v = list.begin(); v != list.end(); v++)
				{
					m_Points[std::distance(list.begin(), v)] = *v;
				}
				m_Size = list.size();

				return *this;
			}

			void PushFront(sf::Vector2f point)
			{
				m_Points = { point, m_Points[0], m_Points[1] };
				m_Size = std::min(m_Size + 1, 3u);
			}

			sf::Vector2f& operator[](unsigned i) { return m_Points[i]; }
			unsigned int Size() const { return m_Size; }

			auto begin() const { return m_Points.begin(); }
			auto end()   const { return m_Points.end() - (3 - m_Size); }
		};

		bool HandleSimplex(Simplex& points, sf::Vector2f& direction);

		bool LineCase(Simplex& points, sf::Vector2f& direction);

		bool TriangleCase(Simplex& points, sf::Vector2f& direction);

		sf::Vector2f FindFurthestPointTriangle(const std::array<sf::Vector2f, 3>* triangle, sf::Vector2f direction);

		sf::Vector2f SupportFunction(
			const ColliderItem* colliderA, const Transform& transformA,
			const ColliderItem* colliderB, const Transform& transformB, 
			sf::Vector2f direction);

		CollisionManifold AlgorithmWithEPA(
			const ColliderItem* colliderA, const Transform& transformA,
			const ColliderItem* colliderB, const Transform& transformB);

		bool Algorithm(
			const ColliderItem* colliderA, const Transform& transformA,
			const ColliderItem* colliderB, const Transform& transformB);
	}

	namespace EPA {

		const size_t MAX_EPA_ITERATIONS = 1000;
		const float TOLERANCE = 0.005f;
		const float DISTANCE_TOLERANCE = 0.01f;

		struct EPAEdge
		{
			sf::Vector2f A;
			sf::Vector2f B;
			sf::Vector2f Normal;
			float Distance;
			size_t AIndex;
		};

		EPAEdge FindClosestEdge(const std::vector<sf::Vector2f>& polytope);

		CollisionManifold GetMTV(const ColliderItem* colliderA, const Transform& transformA,
			const ColliderItem* colliderB, const Transform& transformB,
			const GJK::Simplex& simplex);
	}

	namespace SAT {
		bool Algorithm(
			const std::vector<sf::Vector2f>& polygonA,
			const std::vector<sf::Vector2f>& polygonB);

		bool Algorithm(
			const std::vector<sf::Vector2f>& polygon,
			const std::array<sf::Vector2f, 3>& triangle);

		bool Algorithm(
			const std::array<sf::Vector2f, 3>& triangleA,
			const std::array<sf::Vector2f, 3>& triangleB);
	}

	namespace ClippingAlgo {

		struct CPEdge
		{
			sf::Vector2f A;
			sf::Vector2f B;
			sf::Vector2f Normal;
			float Distance;
			sf::Vector2f Max;
		};

		struct ClippedPoints
		{
			sf::Vector2f Points[2];
			int Size = 0;
		};

		ClippedPoints Clip(const sf::Vector2f& vertex1, const sf::Vector2f& vertex2, const sf::Vector2f& refVec, float dotProduct);
	}
	namespace CollisionDetection {
		CollisionManifold FindCircleCircleCollisionPoints(
			const CircleCollider* circleA, const Transform& transformA,
			const CircleCollider* circleB, const Transform& transformB);

		CollisionManifold FindCirclePlaneCollisionPoints(
			const CircleCollider* circleA, const Transform& circleATransform,
			const PlaneCollider* planeA, const Transform& planeBTransform);

		CollisionManifold FindPlaneCircleCollisionPoints(
			const PlaneCollider* planeA, const Transform& planeATransform,
			const CircleCollider* circleB, const Transform& circleBTransform);

		CollisionManifold FindPlanePlaneCollisionPoints(
			const PlaneCollider* planeA, const Transform& planeATransform,
			const PlaneCollider* planeB, const Transform& planeBTransform);

		CollisionManifold FindPolygonPolygonCollisionPoints(
			const PolygonCollider* polygonA, const Transform& polygonATransform,
			const PolygonCollider* polygonB, const Transform& polygonBTransform);

		CollisionManifold FindPolygonCircleCollisionPoints(
			const PolygonCollider* polygonA, const Transform& polygonATransform,
			const CircleCollider* circleB, const Transform& circleBTransform);

		CollisionManifold FindCirclePolygonCollisionPoints(
			const CircleCollider* circleA, const Transform& circleBTransform,
			const PolygonCollider* polygonB, const Transform& polygonBTransform);

		CollisionManifold FindPolygonPlaneCollisionPoints(
			const PolygonCollider* polygonA, const Transform& polygonATransform,
			const PlaneCollider* planeB, const Transform& planeBTransform);

		CollisionManifold FindPlanePolygonCollisionPoints(
			const PlaneCollider* planeA, const Transform& planeATransform,
			const PolygonCollider* polygonB, const Transform& polygonBTransform);
	}
}