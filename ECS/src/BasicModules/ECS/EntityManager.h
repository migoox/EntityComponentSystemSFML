#pragma once
#include <stack>
#include <array>
#include <cassert>
#include "ECSModules.h"

namespace Basic {
	class EntityManager
	{
	private:
		std::stack<Entity> m_AvailableEntites{};

		std::array<Signature, MAX_ENTITIES> m_EntitySignatures{};

		std::bitset<MAX_ENTITIES> m_EntityVisibility;

		std::bitset<MAX_ENTITIES> m_EntityActivity;

		Entity m_EntitiesCount = 0;

		static EntityManager s_Instance;

	private:
		EntityManager();

		Entity ICreateEntity();
		void IDestroyEntity(Entity entity);

		Entity IEntitiesCount();

		Signature& IEntitySignature(Entity entity);

		bool IGetVisibililty(Entity entity);
		void ISetVisibililty(Entity entity, bool visibility);

		bool IGetActivity(Entity entity);
		void ISetActivity(Entity entity, bool activity);

	public:

		static Entity CreateEntity();
		static void DestroyEntity(Entity entity);

		static Entity EntitiesCount();

		static Signature& EntitySignature(Entity entity);

		static bool GetVisibililty(Entity entity);
		static void SetVisibililty(Entity entity, bool visibility);

		static bool GetActivity(Entity entity);
		static void SetActivity(Entity entity, bool activity);
	};
} // end of Basic