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

				// prepare needed values
				float mA = rigidBodyA.Mass, mB = rigidBodyB.Mass; // mass A and mass B

				float iA = colliderA.Item->GetMomentOfInertia(rigidBodyA); // inertia A
				float iB = colliderB.Item->GetMomentOfInertia(rigidBodyB); // intertia B

				sf::Vector2f vA1 = rigidBodyA.Velocity, vB1 = rigidBodyB.Velocity; // linear velocity before collision
				float omegaA1 = rigidBodyA.AngleVelocity, omegaB1 = rigidBodyB.AngleVelocity; // angle velocity before collision

				sf::Vector2f vA2, vB2; // linear velocity after collision
				float omegaA2, omegaB2; // angle velocity after collision

				sf::Vector2f uA1, uB1; // velocity at collision point before impulse

				sf::Vector2f vAB1; // relative velocity normalized in point of contact

				// centers of gravity
				sf::Vector2f gA = colliderA.Item->GetGlobalCenterOfGravity(collision.ObjectA.GetTransform());
				sf::Vector2f gB = colliderB.Item->GetGlobalCenterOfGravity(collision.ObjectB.GetTransform());

				sf::Vector2f n = collision.Points.Normal; // collision normal

				using MathFunctions::Cross;
				using MathFunctions::Dot;

				// find out which of the items are solid and fix their positions
				if (colliderA.Item->Movable && colliderB.Item->Movable)
				{
					// find point of contact:
					sf::Vector2f pointOfContact = collision.Points.A + collision.Points.Depth * collision.Points.Normal / 2.0f;

					// vector from center of gravity to point of contact for both bodies
					sf::Vector2f rA = pointOfContact - gA, rB = pointOfContact - gB;

					// count velocity in point of contact
					uA1 = vA1 + Cross(omegaA1, rA);
					uB1 = vB1 + Cross(omegaB1, rB);

					// relative velocity
					vAB1 = uA1 - uB1;

					if (Dot(vAB1, n) >= 0.0f) // a and b are not moving towards each other
						return;

					if (colliderA.Item->Rotatable && colliderB.Item->Rotatable)
					{
						// counting impulse parameter
						float j = -(2.0f * Dot(vAB1, n) / (1.0f / mA + 1.0f / mB + std::pow(Cross(rA, n), 2) / iA + std::pow(Cross(rB, n), 2) / iB));

						// counting linear velocity after collision
						vA2 = vA1 + j * n / mA;
						vB2 = vB1 - j * n / mB;

						// giving velocities to rigid bodies
						rigidBodyA.Velocity = vA2;
						rigidBodyB.Velocity = vB2;

						// counting angular velocity after collision
						omegaA2 = omegaA1 + Cross(rA, j * n) / iA;
						omegaB2 = omegaB1 - Cross(rB, j * n) / iB;

						rigidBodyA.AngleVelocity = omegaA2;
						rigidBodyB.AngleVelocity = omegaB2;

					}
					else if (colliderA.Item->Rotatable)
					{
						// counting impulse parameter
						float j = -(2.0f * Dot(vAB1, n) / (1.0f / mA + 1.0f / mB + std::pow(Cross(rA, n), 2) / iA));

						// counting linear velocity after collision
						vA2 = vA1 + j * n / mA;
						vB2 = vB1 - j * n / mB;

						// giving velocities to rigid bodies
						rigidBodyA.Velocity = vA2;
						rigidBodyB.Velocity = vB2;

						// counting angular velocity after collision
						omegaA2 = omegaA1 + Cross(rA, j * n) / iA;

						rigidBodyA.AngleVelocity = omegaA2;
					}
					else if (colliderB.Item->Rotatable)
					{
						// counting impulse parameter
						float j = -(2.0f * Dot(vAB1, n) / (1.0f / mA + 1.0f / mB + std::pow(Cross(rB, n), 2) / iB));

						// counting linear velocity after collision
						vA2 = vA1 + j * n / mA;
						vB2 = vB1 - j * n / mB;

						// giving velocities to rigid bodies
						rigidBodyA.Velocity = vA2;
						rigidBodyB.Velocity = vB2;

						// counting angular velocity after collision
						omegaB2 = omegaB1 + Cross(rB, j * n) / iB;

						rigidBodyA.AngleVelocity = omegaB2;
					}
					else
					{
						// counting impulse parameter
						float j = -(2.0f * Dot(vAB1, n) / (1.0f / mA + 1.0f / mB));

						// counting linear velocity after collision
						vA2 = vA1 + j * n / mA;
						vB2 = vB1 - j * n / mB;

						// giving velocities to rigid bodies
						rigidBodyA.Velocity = vA2;
						rigidBodyB.Velocity = vB2;
					}
				}
				else if (colliderA.Item->Movable)
				{
					// find point of contact:
					sf::Vector2f pointOfContact = collision.Points.B;

					// vector from center of gravity to point of contact for both bodies
					sf::Vector2f rA = pointOfContact - gA, rB = pointOfContact - gB;

					// count velocity in point of contact
					uA1 = vA1 + Cross(omegaA1, rA);
					uB1 = vB1 + Cross(omegaB1, rB);

					// relative velocity
					vAB1 = uA1 - uB1;

					if (Dot(vAB1, n) >= 0.0f) // a and b are not moving towards each other
						return;

					if (colliderA.Item->Rotatable && colliderB.Item->Rotatable)
					{
						// counting impulse parameter
						float j = -(2.0f * Dot(vAB1, n) / (1.0f / mA + std::pow(Cross(rA, n), 2) / iA + std::pow(Cross(rB, n), 2) / iB));

						// counting linear velocity after collision
						vA2 = vA1 + j * n / mA;

						// giving velocities to rigid bodies
						rigidBodyA.Velocity = vA2;

						// counting angular velocity after collision
						omegaA2 = omegaA1 + Cross(rA, j * n) / iA;
						omegaB2 = omegaB1 - Cross(rB, j * n) / iB;

						rigidBodyA.AngleVelocity = omegaA2;
						rigidBodyB.AngleVelocity = omegaB2;

					}
					else if (colliderA.Item->Rotatable)
					{
						// counting impulse parameter
						float j = -(2.0f * Dot(vAB1, n) / (1.0f / mA + std::pow(Cross(rA, n), 2) / iA));

						// counting linear velocity after collision
						vA2 = vA1 + j * n / mA;

						// giving velocities to rigid bodies
						rigidBodyA.Velocity = vA2;

						// counting angular velocity after collision
						omegaA2 = omegaA1 + Cross(rA, j * n) / iA;

						rigidBodyA.AngleVelocity = omegaA2;
					}
					else if (colliderB.Item->Rotatable)
					{
						// counting impulse parameter
						float j = -(2.0f * Dot(vAB1, n) / (1.0f / mA + std::pow(Cross(rB, n), 2) / iB));

						// counting linear velocity after collision
						vA2 = vA1 + j * n / mA;

						// giving velocities to rigid bodies
						rigidBodyA.Velocity = vA2;

						// counting angular velocity after collision
						omegaB2 = omegaB1 - Cross(rB, j * n) / iB;

						rigidBodyB.AngleVelocity = omegaB2;
					}
					else
					{
						// counting impulse parameter
						float j = -(2.0f * Dot(vAB1, n) / (1.0f / mA));

						// counting linear velocity after collision
						vA2 = vA1 + j * n / mA;

						// giving velocities to rigid bodies
						rigidBodyA.Velocity = vA2;
					}
				}
				else if (colliderB.Item->Movable)
				{
					// find point of contact:
					sf::Vector2f pointOfContact = collision.Points.A;

					// vector from center of gravity to point of contact for both bodies
					sf::Vector2f rA = pointOfContact - gA, rB = pointOfContact - gB;

					// count velocity in point of contact
					uA1 = vA1 + Cross(omegaA1, rA);
					uB1 = vB1 + Cross(omegaB1, rB);

					// relative velocity
					vAB1 = uA1 - uB1;

					if (Dot(vAB1, n) >= 0.0f) // a and b are not moving towards each other
						return;

					if (colliderA.Item->Rotatable && colliderB.Item->Rotatable)
					{
						// counting impulse parameter
						float j = -(2.0f * Dot(vAB1, n) / (1.0f / mB + std::pow(Cross(rA, n), 2) / iA + std::pow(Cross(rB, n), 2) / iB));

						// counting linear velocity after collision
						vB2 = vB1 + j * n / mB;

						// giving velocities to rigid bodies
						rigidBodyB.Velocity = vB2;

						// counting angular velocity after collision
						omegaA2 = omegaA1 + Cross(rA, j * n) / iA;
						omegaB2 = omegaB1 - Cross(rB, j * n) / iB;

						rigidBodyA.AngleVelocity = omegaA2;
						rigidBodyB.AngleVelocity = omegaB2;

					}
					else if (colliderA.Item->Rotatable)
					{
						// counting impulse parameter
						float j = -(2.0f * Dot(vAB1, n) / (1.0f / mB + std::pow(Cross(rA, n), 2) / iA));

						// counting linear velocity after collision
						vB2 = vB1 + j * n / mB;

						// giving velocities to rigid bodies
						rigidBodyB.Velocity = vB2;

						// counting angular velocity after collision
						omegaA2 = omegaA1 + Cross(rA, j * n) / iA;

						rigidBodyA.AngleVelocity = omegaA2;
					}
					else if (colliderB.Item->Rotatable)
					{
						// counting impulse parameter
						float j = -(2.0f * Dot(vAB1, n) / (1.0f / mB + std::pow(Cross(rB, n), 2) / iB));

						// counting linear velocity after collision
						vB2 = vB1 + j * n / mB;

						// giving velocities to rigid bodies
						rigidBodyB.Velocity = vB2;

						// counting angular velocity after collision
						omegaB2 = omegaB1 + Cross(rB, j * n) / iB;

						rigidBodyB.AngleVelocity = omegaB2;
					}
					else
					{
						// counting impulse parameter
						float j = -(2.0f * Dot(vAB1, n) / (1.0f / mB));

						// counting linear velocity after collision
						vB2 = vB1 + j * n / mB;

						// giving velocities to rigid bodies
						rigidBodyB.Velocity = vB2;
					}
				}
			}
		}
	};
}