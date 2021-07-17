#pragma once
#include "ECSModules.h"
#include "World.h"

namespace Basic {
	struct GameObject
	{
		Entity ThisEntity;
		World* WorldPtr;

		void Destroy()
		{
			WorldPtr->DestroyEntity(ThisEntity);
		}

		template <typename ComponentType>
		ComponentType& AddComponent(ComponentType&& component)
		{
			return WorldPtr->AddComponent<ComponentType>(ThisEntity, component);
		}

		template <typename ComponentType>
		void RemoveComponent()
		{
			WorldPtr->RemoveComponent<ComponentType>();
		}

		template <typename ComponentType>
		ComponentType& GetComponent()
		{
			return WorldPtr->GetComponent<ComponentType>();
		}
	};
} // end of Basic