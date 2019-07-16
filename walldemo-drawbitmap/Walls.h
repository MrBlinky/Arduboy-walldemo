#pragma once

#include <stdint.h>

enum class Walls : uint8_t
{
	None = 0,
	Above = (1 << 0),
	Side = (1 << 1),
	Below = (1 << 2),
	TopCorner = (1 << 3),
	BottomCorner = (1 << 4),
};

constexpr Walls operator |(Walls left, Walls right)
{
	return static_cast<Walls>(static_cast<uint8_t>(left) | static_cast<uint8_t>(right));
}

inline Walls & operator |=(Walls & left, Walls right)
{
	left = (left | right);
	return left;
}

constexpr Walls operator &(Walls left, Walls right)
{
	return static_cast<Walls>(static_cast<uint8_t>(left) & static_cast<uint8_t>(right));
}

inline Walls & operator &=(Walls & left, Walls right)
{
	left = (left & right);
	return left;
}

constexpr Walls operator ^(Walls left, Walls right)
{
	return static_cast<Walls>(static_cast<uint8_t>(left) ^ static_cast<uint8_t>(right));
}

inline Walls & operator ^=(Walls & left, Walls right)
{
	left = (left ^ right);
	return left;
}