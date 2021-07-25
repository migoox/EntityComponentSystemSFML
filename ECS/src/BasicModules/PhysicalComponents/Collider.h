#pragma once
#include "CircleCollider.h"
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
		Collider(Collider&& other)
		{

			Item = other.Item;
			other.Item = nullptr;
		}
		Collider& operator=(Collider&& other)
		{

			Item = other.Item;
			other.Item = nullptr;

			return *this;
		}

		~Collider()
		{
			if (Item != nullptr)
			{
				std::cout << "delete\n";
				delete Item;
			}
		}
	};
}