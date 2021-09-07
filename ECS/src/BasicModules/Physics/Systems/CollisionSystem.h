#pragma once
#include "../../ECS.h"
#include "../../Components.h"
#include "../../Physics/Solvers/PositionSolver.h"
#include "../../Physics/Solvers/ImpulseSolver.h"
#include "../../Tools/Timer.h"
#include "../../Rendering/VisualGizmos.h"

#include <iostream>

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;

using Basic::CollisionManifold;
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
		AddToSignature<Basic::Collider>();

		// add solvers
		m_Solvers.push_back(std::make_unique<PositionSolver>(m_ParentWorld));
		m_Solvers.push_back(std::make_unique<ImpulseSolver>(m_ParentWorld));
	}

	void Update(const sf::Time& deltaTime) override
	{
		using Basic::Collider;

		// set collision triggered flag as false for all colliders
		for (auto& gameObject : m_GameObjects)
		{
			auto& collider = gameObject.GetComponent<Collider>();

			collider.Item->CollisionTriggered = false;
		}

		// create collision info container
		std::list<CollisionInfo> collisions;

		// iterate trough all game object's
		for (int i = 0; i < m_GameObjects.size(); i++)
		{
			for (int j = i + 1; j < m_GameObjects.size(); j++)
			{
				auto& gameObjectA = m_GameObjects[i];
				auto& gameObjectB = m_GameObjects[j];
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
					CollisionManifold manifold = colliderA->TestCollision(
						transformA, colliderB.Item, transformB);

					// if collision is detected
					if (manifold.HasCollision)
					{
						// raise the flag that collision happened in current frame
						colliderA.Item->CollisionTriggered = true;
						colliderB.Item->CollisionTriggered = true;

						// if one of colliders is marked not to solve, skip
						if (!colliderA.Item->Solve || !colliderB.Item->Solve) continue;

						// if collpoints are unresolvable, skip
						if (!manifold.Resolvable) continue;

						// add collision occurance to resolve

						// collision A with B occurance
						//manifold.RefEdgeFlipped = !manifold.RefEdgeFlipped;
						collisions.emplace_back(gameObjectA, gameObjectB, manifold);
					}
				}
			}
		}

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