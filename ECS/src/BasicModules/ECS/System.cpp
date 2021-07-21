#include "System.h"

void Basic::System::TryToRegisterEntity(Entity entity)
{
	Signature entitySignature = EntityManager::EntitySignature(entity);

	// check if entity is not added
	for(auto it : m_GameObjects)
		if (it.ThisEntity == entity)
			return;

	if (m_SignatureType == SignatureType::Inclusive)
	{
		/*
			ex. ENTITY contains Transform, RigidBody and Collider,
			SYSTEM has Transform and Collider
			System will add entity

			ex2. ENTITY contains Transform, RigidBody, Shape,
			SYSTEM has Transform and Collider
			System won't add entity
		*/
		if (m_Signature == (m_Signature & entitySignature)) // logical operator AND
		{
			GameObject gO = { entity , m_ParentWorld };
			m_GameObjects.push_back(gO);
		}
	}
	else
	{
		/*
			Entity contains the same components as system
		*/
		if (m_Signature == entitySignature)
		{
			GameObject gO = { entity , m_ParentWorld };
			m_GameObjects.push_back(gO);
		}
	}
}

void Basic::System::UpdateRegistration(Entity entity)
{
	Signature entitySignature = EntityManager::EntitySignature(entity);

	// check if entity is not added and save iterator in case it is
	auto it = m_GameObjects.begin();
	for (auto element : m_GameObjects)
	{
		if (element.ThisEntity == entity)
			break;
		it++;
	}

	// entity exists in system, we have to check if it has valid signature
	if (it != m_GameObjects.end())
	{
		if (m_SignatureType == SignatureType::Inclusive)
		{
			if (m_Signature != (m_Signature & entitySignature))
			{
				m_GameObjects.erase(it);
			}
		}
		else
		{
			if (m_Signature != entitySignature)
			{
				m_GameObjects.erase(it);
			}
		}
		return; // end function
	}

	// entity doesn't exist in system, we have to check if it is possible to add it
	if (m_SignatureType == SignatureType::Inclusive)
	{
		if (m_Signature == (m_Signature & entitySignature)) // logical operator AND
		{
			m_GameObjects.push_back({ entity , m_ParentWorld });
		}
	}
	else
	{
		if (m_Signature == entitySignature)
		{
			m_GameObjects.push_back({ entity , m_ParentWorld });
		}
	}
}

void Basic::System::TryToUnregisterEntity(Entity entity)
{
	if (m_GameObjects.size() > 0)
	{
		auto it = m_GameObjects.begin();
		for (auto element : m_GameObjects)
		{
			if (element.ThisEntity == entity)
				break;
			it++;
		}

		m_GameObjects.erase(it);
	}
}
