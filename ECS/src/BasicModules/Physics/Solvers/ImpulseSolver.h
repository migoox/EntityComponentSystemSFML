#pragma once
#include "Solver.h"

// components
#include "../Components/Collider.h"
#include "../Components/RigidBody.h"

// maths
#include "../../Maths/Maths.h"

#include <iostream>

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
			using Maths::Cross;
			using Maths::Dot;

			SetSignature();

			for (auto& collision : collisions)
			{
				// check signature
				if (!HasCorrectSignature(collision.ObjectA) || !HasCorrectSignature(collision.ObjectB)) continue;

				// get colliders
				auto& colliderA = collision.ObjectA.GetComponent<Collider>();
				auto& colliderB = collision.ObjectB.GetComponent<Collider>();

				// get rigid bodies
				auto& rigidBodyA = collision.ObjectA.GetComponent<RigidBody>();
				auto& rigidBodyB = collision.ObjectB.GetComponent<RigidBody>();

				// get transforms
				auto& transformA = collision.ObjectA.GetTransform();
				auto& transformB = collision.ObjectB.GetTransform();

				sf::Vector2f contactPoint;
				if (collision.Manifold.Points.Size() == 2)
				{
					contactPoint = collision.Manifold.Points[0] + 
						(collision.Manifold.Points[1] - collision.Manifold.Points[0]) / 2.0f;
				}
				else
				{
					contactPoint = collision.Manifold.Points[0];
				}

				// masses of both objects
				float mA = rigidBodyA.Mass, mB = rigidBodyB.Mass;

				// moments of inertia of both objects
				float iA = colliderA->GetMomentOfInertia(rigidBodyA);
				float iB = colliderB->GetMomentOfInertia(rigidBodyB);

				// distance vectors from centroid to contact point
				sf::Vector2f rAP = contactPoint - 
					colliderA->GetGlobalCenterOfGravity(transformA);
				sf::Vector2f rBP = contactPoint - 
					colliderB->GetGlobalCenterOfGravity(transformB);

				// current angle velocities
				float omegaA1 = rigidBodyA.AngleVelocity, omegaB1 = rigidBodyB.AngleVelocity;

				// current velocities
				sf::Vector2f vA1 = rigidBodyA.Velocity, vB1 = rigidBodyB.Velocity;

				// velocities in contact point
				sf::Vector2f vAP1, vBP1;

				// relative velocity
				sf::Vector2f vAB1;

				// reference edge normal
				sf::Vector2f n = collision.Manifold.RefEdgeNormal;

				// 1. Count velocity of both bodies in collision point
				vAP1 = Cross(rAP, omegaA1) + vA1;
				vBP1 = Cross(rBP, omegaB1) + vB1;

				// 2. Count relative velocity
				if (collision.Manifold.RefEdgeFlipped)
				{
					// B body has reference edge
					vAB1 = vAP1 - vBP1;
				}
				else
				{
					// A body has reference edge
					vAB1 = vBP1 - vAP1;
				}

				// 3. Count impulse parameter
				float j = (-2.0f) * Dot(vAB1, n);

				if (colliderA->Movable && mA > 0.0f)
					j = j / (1 / mA);
				if (colliderB->Movable && mB > 0.0f)
					j = j / (1 / mB);
				if (colliderA->Rotatable && iA > 0.0f)
					j = j / (std::pow(Cross(rAP, n), 2) / iA);
				if (colliderB->Rotatable && iB > 0.0f)
					j = j / (std::pow(Cross(rBP, n), 2) / iB);

				// 4. Count velocity for object A
				if(colliderA->Movable)
					rigidBodyA.Velocity = vA1 + j / mA * n;
				if(colliderB->Rotatable)
					rigidBodyA.AngleVelocity = omegaA1 + Cross(rAP, j * n);
			}
		}
	};
}