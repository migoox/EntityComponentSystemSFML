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

		bool IsVisible()
		{
			return EntityManager::GetVisibililty(ThisEntity);
		}

		void SetVisibility(bool visibility)
		{
			EntityManager::SetVisibililty(ThisEntity, visibility);
		}

		bool IsActive()
		{
			return EntityManager::GetActivity(ThisEntity);
		}

		void SetActivity(bool activity)
		{
			EntityManager::SetActivity(ThisEntity, activity);
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
			return WorldPtr->GetComponent<ComponentType>(ThisEntity);
		}
	};
} // end of Basic