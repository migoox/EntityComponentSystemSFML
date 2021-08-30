#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "../../Transform.h"
#include "../../ECS/GameObject.h"
#include "../Components/RigidBody.h"
#include "../../Rendering/VisualGizmos.h"

namespace Basic {
	class ContactPoints
	{
	private:
		std::array<sf::Vector2f, 2> m_Points;

		int m_Size = 0;

	public:
		ContactPoints()
			: m_Points({ sf::Vector2f(), sf::Vector2f() }), m_Size(0) {}

		ContactPoints(std::initializer_list<sf::Vector2f> list)
			: m_Points({ sf::Vector2f(), sf::Vector2f() }), m_Size(0)
		{
			*this = list;
		}

		ContactPoints& operator=(std::initializer_list<sf::Vector2f> list)
		{
			int i = 0;
			for (auto& it : list)
			{
				if (i == 2)
				{
					break;
				}

				m_Points[i] = it;
				
				i++;
			}
			m_Size = i;

			return *this;
		}

		void PushBack(const sf::Vector2f& value)
		{
			m_Points = { m_Points[1], value };

			m_Size = std::min(m_Size + 1, 2);
		}

		void PushBack(sf::Vector2f&& value)
		{
			m_Points = { m_Points[1], std::move(value) };

			m_Size = std::min(m_Size + 1, 2);
		}

		sf::Vector2f& operator[](int index)
		{
			if (index >= 2)
				return m_Points[index % 2];
			else if (index < 0)
				return m_Points[index % 2 + 2];
			else
				return m_Points[index];
		}

		const sf::Vector2f& operator[](int index) const
		{
			if (index >= 2)
				return m_Points[index % 2];
			else if (index < 0)
				return m_Points[index % 2 + 2];
			else
				return m_Points[index];
		}

		int Size() const
		{
			return m_Size;
		}
	};

	struct CollisionManifold
	{
		ContactPoints Points;

		sf::Vector2f A;
		sf::Vector2f B;

		sf::Vector2f Normal;
		float Depth = 0.0f;

		bool HasCollision = false;
		bool Resolvable = true;
	};

	struct AABB
	{
		sf::Vector2f minPoint;
		sf::Vector2f maxPoint;
	};

	class PlaneCollider;
	class CircleCollider;
	class PolygonCollider;

	struct ColliderItem
	{
		bool CollisionTriggered = false;

		// if you want to turn off collider set is as false
		bool Active = true;

		// if you don't want this collider to have solved collisions set it as false
		bool Solve = true;

		// if you want to make movement got from collisions of your body blocked set it as false
		bool Movable = true;

		// if you want to make rotation got from collisions of your body blocked set it as false
		bool Rotatable = true;

		// relatively to this center transformation of collider will be done
		virtual sf::Vector2f GetGlobalCenterOfGravity(const Transform& trans) const = 0;

		virtual float GetMomentOfInertia(const RigidBody& rb) const = 0;

		// move collider relatively to game object's transform
		virtual void MoveCollider(sf::Vector2f displacement) = 0;

		// rotate collider around it's center of gravity (angle in degrees)
		virtual void RotateCollider(float angle) { };

		// get rectangle around collider shape
		virtual AABB GetGlobalAABB(const Transform& trans) const = 0;

		virtual void DrawOnceOnVisualGizmos(const Transform& trans) const = 0;

		virtual void DrawAABBOnceOnVisualGizmos(const Transform& trans) const = 0;

		virtual sf::Vector2f FindFurthestPointInDirection(const Transform& transform, sf::Vector2f direction) const = 0;

		virtual CollisionManifold TestCollision(
			const Transform& transform,
			const ColliderItem* circleCollider,
			const Transform& circleTransform) const = 0;

		virtual CollisionManifold TestCollision(
			const Transform& transform,
			const CircleCollider* circleCollider,
			const Transform& circleTransform) const = 0;

		virtual CollisionManifold TestCollision(
			const Transform& transform,
			const PlaneCollider* planeCollider,
			const Transform& planeTransform) const = 0;

		virtual CollisionManifold TestCollision(
			const Transform& transform,
			const PolygonCollider* polygonCollider,
			const Transform& polygonTransform) const = 0;
	};

	struct CollisionInfo
	{
		GameObject ObjectA;
		GameObject ObjectB;
		CollisionManifold Manifold;

		CollisionInfo(GameObject objA, GameObject objB, CollisionManifold points)
			: ObjectA(objA), ObjectB(objB), Manifold(points) { }
	};
}