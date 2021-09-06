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
				sf::Vector2f n = collision.Manifold.Normal;

				// 1. Count velocity of both bodies in collision point
				vAP1 = Cross(rAP, omegaA1) + vA1;
				vBP1 = Cross(rBP, omegaB1) + vB1;

				// 2. Count relative velocity
				vAB1 = vAP1 - vBP1;

				// 3. Count bounciness factor
				float bouncinessFactor = 1.0f;

				if (collision.ObjectA.HasComponent<PhysicMaterial>() && collision.ObjectB.HasComponent<PhysicMaterial>())
				{
					auto& matA = collision.ObjectA.GetComponent<PhysicMaterial>();
					auto& matB = collision.ObjectB.GetComponent<PhysicMaterial>();

					bouncinessFactor = (matA.Bounciness + matB.Bounciness) / 2.0f;
				}
				else if (collision.ObjectA.HasComponent<PhysicMaterial>())
				{
					auto& matA = collision.ObjectA.GetComponent<PhysicMaterial>();
					bouncinessFactor = (matA.Bounciness + 1.0f) / 2.0f;
				}
				else if (collision.ObjectB.HasComponent<PhysicMaterial>())
				{
					auto& matB = collision.ObjectB.GetComponent<PhysicMaterial>();
					bouncinessFactor = (matB.Bounciness + 1.0f) / 2.0f;
				}
				
				// 4. Count impulse parameter
				float j = -(1.0f + bouncinessFactor) * Dot(vAB1, n);
				float denominator = 0.0f;

				if (colliderA->Movable && mA > 0.0f)
					denominator += (1.0f / mA);
				if (colliderB->Movable && mB > 0.0f)
					denominator += (1.0f / mB);
				if (colliderA->Rotatable && iA > 0.0f)
					denominator += (std::pow(Cross(rAP, n), 2) / iA);
				if (colliderB->Rotatable && iB > 0.0f)
					denominator += (std::pow(Cross(rBP, n), 2) / iB);

				if (denominator == 0.0f) continue;

				j /= denominator;

				// 5. Count velocities
				if(colliderA->Movable)
					rigidBodyA.Velocity = vA1 + j * n / mA;
				if(colliderA->Rotatable)
					rigidBodyA.AngleVelocity = omegaA1 + Cross(rAP, j * n) / iA;

				if (colliderB->Movable)
					rigidBodyB.Velocity = vB1 - j * n / mB;
				if (colliderB->Rotatable)
					rigidBodyB.AngleVelocity = omegaB1 - Cross(rBP, j * n) / iB;
			}
		}
	};
}