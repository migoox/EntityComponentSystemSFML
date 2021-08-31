#pragma once
#include "../Colliders/ColliderItem.h"

namespace Basic {
	struct Collider
	{
		ColliderItem* Item;
		Collider()
		{
			Item = nullptr;
		}

		Collider(ColliderItem* item) : Item(item) { }

		Collider(const Collider& other) = default;
		Collider& operator=(const Collider& other) = default;
		Collider(Collider&& other) noexcept
		{

			Item = other.Item;
			other.Item = nullptr;
		}
		Collider& operator=(Collider&& other) noexcept
		{

			Item = other.Item;
			other.Item = nullptr;

			return *this;
		}

		ColliderItem* operator->() const
		{
			return Item;
		}

		~Collider()
		{
			if (Item != nullptr)
			{
				delete Item;
			}
		}
	};
}