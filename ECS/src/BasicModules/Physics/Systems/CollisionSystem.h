#pragma once
#include "../../ECS.h"
#include "../../Components.h"
#include "../../Physics/Solvers/PositionSolver.h"
#include "../../Physics/Solvers/ImpulseSolver.h"
#include "../../Tools/Timer.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;

using Basic::CollisionPoints;
using Basic::CollisionInfo;

using Basic::Solver;
using Basic::PositionSolver;
using Basic::ImpulseSolver;

class CollisionSystem : public ECSSystem
{
private:
	std::vector<std::unique_ptr<Solver>> m_Solvers;


public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<Collider>();

		// add solvers
		m_Solvers.push_back(std::make_unique<PositionSolver>(m_ParentWorld));
		//m_Solvers.push_back(std::make_unique<ImpulseSolver>(m_ParentWorld));
	}

	void Update(const sf::Time& deltaTime) override
	{
		// set collision triggered flag as false for all colliders
		for (auto& gameObject : m_GameObjects)
		{
			auto& collider = gameObject.GetComponent<Collider>();

			collider.Item->CollisionTriggered = false;
		}

		// create collision info container
		std::list<CollisionInfo> collisions;

		auto colliders = m_ComponentManager->ArrayData<Collider>();
		size_t collidersCount = m_ComponentManager->Size<Collider>();
		if (colliders != nullptr)
		{
			for (size_t i = 0; i < collidersCount; i++)
			{
				for (size_t j = i + 1; j < collidersCount; j++)
				{
					auto gameObjectA = m_ParentWorld->GetGameObject(m_ComponentManager->GetEntityByComponentArrayIndex<Collider>(i));
					auto gameObjectB = m_ParentWorld->GetGameObject(m_ComponentManager->GetEntityByComponentArrayIndex<Collider>(j));
					if (gameObjectA.IsActive() && gameObjectB.IsActive())
					{
						// get colliders
						auto& colliderA = colliders[i];
						auto& colliderB = colliders[j];

						// if one of colliders is blank, skip
						if (colliderA.Item == nullptr || colliderB.Item == nullptr) continue;

						// if one of colliders is not active, skip
						if (!colliderA->Active || !colliderB->Active) continue;

						// get transforms
						auto& transformA = gameObjectA.GetTransform();
						auto& transformB = gameObjectB.GetTransform();

						// collider A collides with collider B and returns collision points
						CollisionPoints collPoints1 = colliderA->TestCollision(
							transformA, colliderB.Item, transformB);

						// if collision is detected
						if (collPoints1.HasCollision)
						{
							// raise the flag that collision happened in current frame
							colliderA.Item->CollisionTriggered = true;
							colliderB.Item->CollisionTriggered = true;

							// if one of colliders is marked not to solve, skip
							if (!colliderA.Item->Solve || !colliderB.Item->Solve) continue;

							// if collpoints are unresolvable, skip
							if (!collPoints1.Resolvable) continue;

							// add collision occurance to resolve

							// collision A with B occurance
							collisions.emplace_back(gameObjectA, gameObjectB, collPoints1);

							// reversed collision points
							CollisionPoints collPoints2; 
							collPoints2.A = collPoints1.B;
							collPoints2.B = collPoints1.A;
							collPoints2.Depth = collPoints1.Depth;
							collPoints2.Normal = -collPoints1.Normal;
							collPoints2.HasCollision = true;

							// collision B with A occurance
							collisions.emplace_back(gameObjectB, gameObjectA, collPoints2);
						}
					}

				}
			}
		}

		/* slow alternative
		// collision detection loop
		for (auto gameObjectA : m_GameObjects)
		{
			for (auto gameObjectB : m_GameObjects)
			{
				// if objects are the same, skip
				if (gameObjectA == gameObjectB) continue;

				if (gameObjectA.IsActive() && gameObjectB.IsActive())
				{
					// get colliders
					auto& colliderA = gameObjectA.GetComponent<Collider>();
					auto& colliderB = gameObjectB.GetComponent<Collider>();

					// if one of colliders is blank, skip
					if (colliderA.Item == nullptr || colliderB.Item == nullptr) continue;

					// if one of colliders is not active, skip
					if (!colliderA->Active || !colliderB->Active) continue;

					// get transforms
					auto& transformA = gameObjectA.GetTransform();
					auto& transformB = gameObjectB.GetTransform();

					// collider A collides with collider B and returns collision points
					CollisionPoints collPoints = colliderA->TestCollision(
						transformA, colliderB.Item, transformB);

					// if collision is detected
					if (collPoints.HasCollision)
					{
						// raise the flag that collision happened in current frame
						colliderA.Item->CollisionTriggered = true;
						colliderB.Item->CollisionTriggered = true;

						// if one of colliders is marked not to solve, skip
						if (!colliderA.Item->Solve || !colliderB.Item->Solve) continue;

						// if collpoints are unresolvable, skip
						if (!collPoints.Resolvable) continue;


						// add collision occurance to resolve
						collisions.emplace_back(gameObjectA, gameObjectB, collPoints);
					}
				}
			}
		}
		*/

		// collision resolving
		for (auto& solver : m_Solvers)
			solver->Solve(collisions, deltaTime);
	}

	void OnGameObjectAdded(GameObject gameObject)
	{
	}

	void OnGameObjectRemoved(GameObject gameObject)
	{
	}
};