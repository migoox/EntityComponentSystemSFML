#pragma once
#include <math.h>
#include <array>

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
				: m_Points({ sf::Vector2f(0.f, 0.0f), sf::Vector2f(0.f, 0.0f), sf::Vector2f(0.f, 0.0f) }), m_Size(0)
			{}

			Simplex(std::initializer_list<sf::Vector2f> list)
				: m_Points({ sf::Vector2f(0.f, 0.0f), sf::Vector2f(0.f, 0.0f), sf::Vector2f(0.f, 0.0f) }), m_Size(0)
			{
				*this = list;
			}

			Simplex& operator=(std::initializer_list<sf::Vector2f> list)
			{
				int i = 0;
				// iterate trough the list
				for (auto& it : list)
				{
					// if list's size is bigger than 3
					// break the loop
					if (i > 3)
						break;

					m_Points[i] = it;
					i++;
				}

				if (i > 3)
					m_Size = 3;
				else
					m_Size = list.size();

				return *this;
			}

			void PushFront(sf::Vector2f point)
			{
				m_Points = { point, m_Points[0], m_Points[1] };
				m_Size = std::min(m_Size + 1, 3u);
			}

			void PopBack()
			{
				assert(m_Size > 0 && "Simplex out of range.");

				m_Size--;

				if (m_Size == 0)
					m_Points = { };
				else if(m_Size == 1)
					m_Points = { m_Points[0] };
				else
					m_Points = { m_Points[0], m_Points[1] };
			}

			sf::Vector2f& operator[](unsigned i) { return m_Points[i]; }
			unsigned int Size() const { return m_Size; }

			auto begin() const { return m_Points.begin(); }
			auto end()   const { return m_Points.end() - (3 - m_Size); }
		};

		bool HandleSimplex(Simplex points, sf::Vector2f direction);

		bool LineCase(Simplex& points, sf::Vector2f& direction);

		bool TriangleCase(Simplex& points, sf::Vector2f& direction);

		sf::Vector2f FindFurthestPointTriangle(const std::array<sf::Vector2f, 3>* triangle, sf::Vector2f direction);

		sf::Vector2f SupportFunction(const ColliderItem* colliderA, const Transform& transformA,
			const ColliderItem* colliderB, const Transform& transformB, sf::Vector2f direction);

		bool Algorithm(const ColliderItem* colliderA, const Transform& transformA,
			const ColliderItem* colliderB, const Transform& transformB);
	}
	namespace SAT {
		bool Algorithm(
			const std::vector<sf::Vector2f>* polygonA,
			const std::vector<sf::Vector2f>* polygonB);

	}
	namespace CollisionDetection {
		CollisionPoints FindCircleCircleCollisionPoints(
			const CircleCollider* circle1, const Transform& transform1,
			const CircleCollider* circle2, const Transform& transform2);

		CollisionPoints FindCirclePlaneCollisionPoints(
			const CircleCollider* circle, const Transform& circleTransform,
			const PlaneCollider* plane, const Transform& planeTransform);

		CollisionPoints FindPlaneCircleCollisionPoints(
			const PlaneCollider* plane, const Transform& planeTransform,
			const CircleCollider* circle, const Transform& circleTransform);

		CollisionPoints FindPlanePlaneCollisionPoints(
			const PlaneCollider* plane1, const Transform& plane1Transform,
			const PlaneCollider* plane2, const Transform& plane2Transform);

		CollisionPoints FindPolygonPolygonCollisionPoints(
			const PolygonCollider* polygon1, const Transform& polygon1Transform,
			const PolygonCollider* polygon2, const Transform& polygon2Transform);

		CollisionPoints FindPolygonCircleCollisionPoints(
			const PolygonCollider* polygon, const Transform& polygonTransform,
			const CircleCollider* circle, const Transform& circleTransform);

		CollisionPoints FindCirclePolygonCollisionPoints(
			const CircleCollider* circle, const Transform& circleTransform,
			const PolygonCollider* polygon, const Transform& polygonTransform);

		CollisionPoints FindPolygonPlaneCollisionPoints(
			const PolygonCollider* polygon, const Transform& polygonTransform,
			const PlaneCollider* plane, const Transform& planeTransform);

		CollisionPoints FindPlanePolygonCollisionPoints(
			const PlaneCollider* plane, const Transform& planeTransform,
			const PolygonCollider* polygon, const Transform& polygonTransform);
	}
}