#pragma once
#include "ECSModules.h"

namespace Basic {
	template <typename ComponentType>
	struct ComponentHandle
	{
		Entity Owner;
		World* WorldPtr;
		ComponentType* Component;

		ComponentType* operator->() const { return Component; }
	};
}