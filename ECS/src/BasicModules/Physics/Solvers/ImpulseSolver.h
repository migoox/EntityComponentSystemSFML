#pragma once
#include "Solver.h"

// components
#include "../Components/Collider.h"
#include "../Components/RigidBody.h"

namespace Basic {
	class ImpulseSolver : public Solver
	{
	private:
		void SetSignature() override
		{
			// collisions will be solved only if both objects contain rigid body
			AddToSignature<Collider>();
			AddToSignature<RigidBody>();
		}

	public:
		using Solver::Solver;

		void Solve(std::list<CollisionInfo>& collisions, const sf::Time& deltaTime) override
		{
			SetSignature();

			for (auto& collision : collisions)
			{
				// check signature
				if (!HasProperSignature(collision.ObjectA) || !HasProperSignature(collision.ObjectB)) continue;

				// get colliders
				auto& colliderA = collision.ObjectA.GetComponent<Collider>();
				auto& colliderB = collision.ObjectB.GetComponent<Collider>();

				// get rigid bodies
				auto& rigidBodyA = collision.ObjectA.GetComponent<RigidBody>();
				auto& rigidBodyB = collision.ObjectB.GetComponent<RigidBody>();

				// prepare velocities in collision normal
				sf::Vector2f velBeforeCollisionA = (-collision.Points.Normal.x * rigidBodyA.Velocity.x + -collision.Points.Normal.y * rigidBodyA.Velocity.y) * (-collision.Points.Normal);;
				sf::Vector2f velBeforeCollisionB = (collision.Points.Normal.x * rigidBodyB.Velocity.x + collision.Points.Normal.y * rigidBodyB.Velocity.y) * (collision.Points.Normal);

				sf::Vector2f velAfterCollisionA;
				sf::Vector2f velAfterCollisionB;

				// find out which of the items are solid and fix their positions
				if (!colliderA.Item->Solid && !colliderB.Item->Solid)
				{
					velAfterCollisionA = (rigidBodyA.Mass * velBeforeCollisionA + 2 * rigidBodyB.Mass * velBeforeCollisionB
						- rigidBodyB.Mass * velBeforeCollisionA) / (rigidBodyA.Mass + rigidBodyB.Mass);

					velAfterCollisionB = velBeforeCollisionA - velBeforeCollisionB + velAfterCollisionA;

					rigidBodyA.Velocity -= velBeforeCollisionA;
					rigidBodyB.Velocity -= velBeforeCollisionB;

					rigidBodyA.Velocity += velAfterCollisionA;
					rigidBodyB.Velocity += velAfterCollisionB;
				}
				else if (!colliderA.Item->Solid)
				{
					velAfterCollisionA = -velBeforeCollisionA;

					rigidBodyA.Velocity -= velBeforeCollisionA;
					rigidBodyA.Velocity += velAfterCollisionA;
				}
				else if (!colliderB.Item->Solid)
				{
					velAfterCollisionB = -velBeforeCollisionB;

					rigidBodyB.Velocity -= velBeforeCollisionB;
					rigidBodyB.Velocity += velAfterCollisionB;
				}
			}
		}
	};
}