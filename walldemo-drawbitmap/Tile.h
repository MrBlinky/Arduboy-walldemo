#pragma once

#include <stdint.h>

enum class Tile : uint8_t
{
	None, Wall,
};

constexpr Tile invert(Tile tile)
{
	return static_cast<Tile>(static_cast<uint8_t>(tile) ^ static_cast<uint8_t>(Tile::Wall));
}

inline void toggle(Tile & tile)
{
	tile = invert(tile);
}