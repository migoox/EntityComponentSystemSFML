#pragma once
#include "ECS.h"
#include "ComponentManager.h"
#include "EntityManager.h"

class World
{
private:
	

public:
	void Init();

	void CreateEntity(Entity entity);
	void DestroyEntity(Entity entity);

	template <typename ComponentType>
	void AddComponent(Entity& entity, ComponentType&& component);

	template <typename ComponentType>
	void RemoveComponent(Entity& entity);

	template<typename ComponentType, typename ...Args>
};