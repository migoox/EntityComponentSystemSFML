#pragma once
#include <bitset>

namespace Basic
{
	using Entity = std::uint32_t;

	const Entity MAX_ENTITIES = 1024;

	using ComponentID = std::uint8_t;

	const ComponentID MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;
} // end of Basic