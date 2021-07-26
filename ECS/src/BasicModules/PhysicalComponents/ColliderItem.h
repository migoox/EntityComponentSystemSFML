#pragma once
#include <SFML/Graphics.hpp>
#include "../OtherComponents/Transform.h"

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
		bool Active = true;
		bool Resolve = false;

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
}