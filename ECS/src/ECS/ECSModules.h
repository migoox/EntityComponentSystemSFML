#pragma once
#include <bitset>

using Entity = std::uint32_t;

const Entity MAX_ENTITIES = 5120;

using ComponentID = std::uint8_t;

const ComponentID MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;