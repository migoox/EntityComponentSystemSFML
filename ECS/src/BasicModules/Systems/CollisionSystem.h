#pragma once
#include <iostream>
#include <windows.h>

#include "../ECS.h"
#include "../Components.h"
#include "../PhysicalComponents/PositionSolver.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;

using Basic::CollisionPoints;
using Basic::CollisionInfo;

using Basic::Solver;
using Basic::PositionSolver;

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

		// collision detection 
		for (size_t i = 0; i < m_GameObjects.size(); i++)
		{
			for (size_t j = i + 1; j < m_GameObjects.size(); j++)
			{
				// get game objects
				auto& gameObject1 = m_GameObjects.at(i);
				auto& gameObject2 = m_GameObjects.at(j);
				
				if (gameObject1.IsActive() && gameObject2.IsActive())
				{
					// get colliders
					auto& collider1 = gameObject1.GetComponent<Collider>();
					auto& collider2 = gameObject2.GetComponent<Collider>();

					// if one of colliders is blank, skip
					if (collider1.Item == nullptr || collider2.Item == nullptr) continue;

					// if one of colliders is not active, skip
					if (!collider1.Item->Active || !collider2.Item->Active) continue;

					// get transforms
					auto& transform1 = gameObject1.GetTransform();
					auto& transform2 = gameObject2.GetTransform();

					// get collision points of both colliders
					CollisionPoints collPoints = collider1.Item->TestCollision(
						transform1, collider2.Item, transform2);

					// if collision is detected
					if (collPoints.HasCollision)
					{
						// raise the flag that collision happened in current frame
						collider1.Item->CollisionTriggered = true;
						collider2.Item->CollisionTriggered = true;

						// if collpoints are unresolvable or both colliders are flagged
						// as solid, or one of them is flagged as not to solve, 
						// do not add any collision info to the list
						if((collider1.Item->Solid && collider2.Item->Solid) ||
							(!collider1.Item->Solve || !collider2.Item->Solve) ||
							!collPoints.Resolvable)
							continue;

						// add collision occurance to resolve
						collisions.emplace_back(gameObject1, gameObject2, collPoints);
					}
				}
			}
		}

		// collision resolving
		for (auto& solver : m_Solvers)
			solver->Solve(collisions, deltaTime);
	}
};