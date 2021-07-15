#pragma once
#include "ECS.h"
#include <queue>
class EntityManager
{
private:
	EntityManager();

	Entity m_EntitiesCount;

	std::queue<Entity> m_AvailableEntitiesIDs;

	static EntityManager s_Instance;

	Entity ICreate();
	void IDestroy(Entity entity);

public:
	EntityManager(const EntityManager&) = delete;

	static Entity Create();
	static void Destroy(Entity entity);

	Entity& operator=(const EntityManager&) = delete;

	static EntityManager& Instance();
};