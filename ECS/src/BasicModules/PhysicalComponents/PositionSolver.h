#pragma once
#include "Solver.h"

namespace Basic {
	class PositionSolver : public Solver
	{
	private:
		void SetSignature() override
		{
			//AddToSignature<Collider>();
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

				// find out which of the items are solid and fix their positions
				if (!colliderA.Item->Solid && !colliderB.Item->Solid)
				{
					sf::Vector2f fixer = collision.Points.Depth * collision.Points.Normal / 2.0f;
					collision.ObjectA.GetTransform().move(fixer);
					collision.ObjectB.GetTransform().move(-fixer);

				}
				else if (!colliderA.Item->Solid)
				{
					sf::Vector2f fixer = collision.Points.Depth * collision.Points.Normal;
					collision.ObjectA.GetTransform().move(fixer);
				}
				else if (!colliderB.Item->Solid)
				{
					sf::Vector2f fixer = collision.Points.Depth * collision.Points.Normal;
					collision.ObjectB.GetTransform().move(-fixer);
				}
			}
		}
	};
}