#pragma once
#include "ECSModules.h"
#include "ComponentManager.h"
template <typename ComponentType>
struct ComponentHandle
{
	Entity Owner;
	ComponentType* ComponentPtr;
	ComponentManager* ComponentManagerPtr;

	ComponentHandle() {};
	ComponentHandle(Entity owner, ComponentType* component, ComponentManager<ComponentType>* manager)
		: Owner(owner), ComponentPtr(component), ComponentManager(manager)
	{
	}

	ComponentType* operator->() const
	{
		return ComponentPtr;
	}

	void Destroy()
	{
		ComponentManagerPtr->RemoveComponent<ComponentType>(Owner);
	}
};