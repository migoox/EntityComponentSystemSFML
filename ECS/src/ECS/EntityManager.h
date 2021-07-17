#pragma once
#include <stack>
#include <array>
#include "ECSModules.h"

class EntityManager
{
private:
	std::stack<Entity> m_AvailableEntites{};

	std::array<Signature, MAX_ENTITIES> m_EntitySignatures{};

	Entity m_EntitiesCount = 0;

	static EntityManager s_Instance;

private:
	EntityManager();

	Entity ICreateEntity();
	void IDestroyEntity(Entity entity);

	Entity IEntitiesCount();

	Signature& IEntitySignature(Entity entity);
public:

	static Entity CreateEntity();
	static void DestroyEntity(Entity entity);

	static Entity EntitiesCount();

	static Signature& EntitySignature(Entity entity);
};