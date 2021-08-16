#pragma once
#include "Transform.h"

// rendering
#include "Rendering/Components/CircleShape.h"
#include "Rendering/Components/RectangleShape.h"
#include "Rendering/Components/ConvexShape.h"
#include "Rendering/Components/VertexArray.h"
#include "Rendering/Components/Line.h"
#include "Rendering/Components/Sprite.h"

// physics 
	// components
#include "Physics/Components/RigidBody.h"
#include "Physics/Components/Collider.h"

	// colliders
#include "Physics/Colliders/CircleCollider.h"
#include "Physics/Colliders/PlaneCollider.h"
#include "Physics/Colliders/PolygonCollider.h"
#include "Physics/Colliders/RectangleCollider.h"

// animation
#include "Animation/Components/Animator.h"
#include "Animation/Animation.h"


namespace Basic {
	namespace Components {
		using Basic::CircleShape;
		using Basic::RectangleShape;
		using Basic::ConvexShape;
		using Basic::VertexArray;
		using Basic::Line;
		using Basic::Sprite;

		using Basic::RigidBody;
		using Basic::Collider;

		using Basic::ColliderItem;
		using Basic::PlaneCollider;
		using Basic::CircleCollider;
		using Basic::PolygonCollider;
		using Basic::RectangleCollider;

		using Basic::Animation;
	}
}

