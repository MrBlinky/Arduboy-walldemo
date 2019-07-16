#pragma once

#include "Walls.h"

// Low nibbles left half wall tile
// High nibbles right half wall tile
const uint8_t PROGMEM wallDecodeTable[16] =
{
	// 0000, 0001, 0010, 0011, 0100, 0101, 0110, 0111
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0xA6, 0xB7,
	
	// 1000, 1001, 1010, 1011, 1100, 1101, 1110, 1111
	0x00, 0x11, 0x22, 0x88, 0x44, 0x55,	0xA6, 0xC9,
};

inline uint8_t wallDecode(Walls walls)
{
	return pgm_read_byte(&wallDecodeTable[static_cast<uint8_t>(walls)]);
}