#pragma once
#include <SFML/Graphics.hpp>
#include "../../Transform.h"
#include "../../ECS/GameObject.h"
#include "../Components/RigidBody.h"
#include "../../Rendering/VisualGizmos.h"

namespace Basic {
	struct CollisionPoints
	{
		sf::Vector2f A;
		sf::Vector2f B;
		sf::Vector2f Normal;
		float Depth = 0.0f;
		bool HasCollision = false;
		bool Resolvable = true;
	};

	struct Triangle
	{
		sf::Vector2f A;
		sf::Vector2f B;
		sf::Vector2f C;
	};

	class PlaneCollider;
	struct CircleCollider;
	struct PolygonCollider;

	struct ColliderItem
	{
		bool CollisionTriggered = false;

		bool Active = true;

		bool Movable = true;
		bool Rotatable = true;

		// relatively to this center transformation of collider will be done
		virtual sf::Vector2f GetGlobalCenterOfGravity(const Transform& trans) const = 0;

		virtual float GetMomentumOfInertia(const RigidBody& rb) const = 0;

		virtual void DrawOnceOnVisualGizmos(const Transform& trans) const = 0;

		virtual CollisionPoints TestCollision(
			const Transform& transform,
			const ColliderItem* circleCollider,
			const Transform& circleTransform) const = 0;

		virtual CollisionPoints TestCollision(
			const Transform& transform,
			const CircleCollider* circleCollider,
			const Transform& circleTransform) const = 0;

		virtual CollisionPoints TestCollision(
			const Transform& transform,
			const PlaneCollider* planeCollider,
			const Transform& planeTransform) const = 0;

		virtual CollisionPoints TestCollision(
			const Transform& transform,
			const PolygonCollider* convexCollider,
			const Transform& convexTransform) const = 0;

	};

	struct CollisionInfo
	{
		CollisionInfo(GameObject objA, GameObject objB, CollisionPoints points)
			: ObjectA(objA), ObjectB(objB), Points(points) { }
		GameObject ObjectA;
		GameObject ObjectB;
		CollisionPoints Points;
	};
}