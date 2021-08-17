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

		virtual void DrawOnceOnVisualGizmos(const Transform& trans) const = 0;

		// move collider relatively to game object's transform
		virtual void MoveCollider(sf::Vector2f displacement) = 0;

		// rotate collider around it's center of gravity (angle in degrees)
		virtual void RotateCollider(float angle) { };

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