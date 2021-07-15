#pragma once
#include "ECS.h"
#include "World.h"
struct GameObject
{
	Entity entity;
	World* world;
	
	void Destroy()
	{
		world->DestroyEntity(entity);
	}

	template <typename ComponentType>
	void AddComponent(ComponentType component)
	{
		world->AddComponent(entity, component);
	}

	template <typename ComponentType>
	void RemoveComponent()
	{
		world->RemoveComponent<ComponentType>(entity);
	}
};