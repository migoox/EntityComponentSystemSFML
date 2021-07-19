#pragma once
#include <map>
#include "ECSModules.h"
namespace Basic {
	class EntityMap
	{
	private:
		std::map<Entity, size_t> m_EntityToArrayIndexMap{};
		std::map<size_t, Entity> m_ArrayIndexToEntityMap{};

		size_t m_Size = 0;

	public:
		EntityMap() = default;

		void AddEntity(Entity entity);
		void RemoveEntity(Entity entity);

		Entity ToEntity(size_t arrayIndex);
		size_t ToArrIndex(Entity entity);

		bool IsContain(Entity entity);

		void Clear();

	};
} // end of Basic