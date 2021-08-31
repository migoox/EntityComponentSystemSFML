#pragma once
#include "Solver.h"
#include <iostream>
namespace Basic {
	class PositionSolver : public Solver
	{
	private:
		void SetSignature() override
		{
			// all collisions will be solved
			AddToSignature<Collider>();
		}

	public:
		using Solver::Solver; // using solver's constructors

		void Solve(std::list<CollisionInfo>& collisions, const sf::Time& deltaTime) override
		{
			SetSignature();

			for (auto& collision : collisions)
			{
				continue;
				// check signature
				if (!HasCorrectSignature(collision.ObjectA) || !HasCorrectSignature(collision.ObjectB)) continue;

				// get colliders
				auto& colliderA = collision.ObjectA.GetComponent<Collider>();
				auto& colliderB = collision.ObjectB.GetComponent<Collider>();

				sf::Vector2f fixer;

				// find out which of the items are solid and fix their positions
				if (colliderA.Item->Movable)
				{
					fixer = collision.Manifold.Depth * collision.Manifold.Normal / 2.0f;
					collision.ObjectA.GetTransform().move(fixer);
				}
				else
				{
					fixer = collision.Manifold.Depth * collision.Manifold.Normal;
					collision.ObjectA.GetTransform().move(fixer);
				}
			}
		}
	};
}