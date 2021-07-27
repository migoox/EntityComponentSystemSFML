#pragma once
#include <SFML/Graphics.hpp>
#include "../OtherComponents/Transform.h"
#include "../ECS/GameObject.h"

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

	struct PlaneCollider;
	struct CircleCollider;
	struct RectangleCollider;
	struct ConvexCollider;

	struct ColliderItem
	{
		bool CollisionTriggered = false;

		bool Active = true;
		bool Solve = true;
		bool Solid = true;

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