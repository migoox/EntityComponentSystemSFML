#pragma once
#include "ECSModules.h"
#include "EntityManager.h"
#include "../OtherComponents/Transform.h"

namespace Basic {
	class World;

	struct GameObject
	{
		Entity ThisEntity;
		World* WorldPtr;

		bool IsVisible() const
		{
			return EntityManager::GetVisibililty(ThisEntity);
		}

		void SetVisibility(bool visibility)
		{
			EntityManager::SetVisibililty(ThisEntity, visibility);
		}

		bool IsActive() const
		{
			return EntityManager::GetActivity(ThisEntity);
		}

		void SetActivity(bool activity)
		{
			EntityManager::SetActivity(ThisEntity, activity);
		}

		bool operator<(const GameObject& rhs) const noexcept
		{
			return ThisEntity < rhs.ThisEntity;
		}

		bool operator==(const GameObject& rhs) const noexcept
		{
			return ThisEntity == rhs.ThisEntity;
		}

		bool operator!=(const GameObject& rhs) const noexcept
		{
			return ThisEntity != rhs.ThisEntity;
		}

		Signature GetSignature() const
		{
			return EntityManager::EntitySignature(ThisEntity);
		}

		// functions underneath are defined in World.h under World class

		void Destroy();

		Transform& GetTransform() const;

		template <typename ComponentType>
		ComponentType& AddComponent(ComponentType&& component);

		template <typename ComponentType>
		void RemoveComponent();

		template <typename ComponentType>
		ComponentType& GetComponent();

		template <typename ComponentType>
		bool HasComponent();
	};
} // end of Basic