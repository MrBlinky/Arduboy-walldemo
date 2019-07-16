// auto wall outline demo by Mr.Blinky July 2019

// use D-Pad to move cursors 
// A button to toggle Wall / no-wall tile
// B button to toggle wall / no-wall border 

#include <Arduboy2.h>
#include "Images.h"

#define WALL 0x01
constexpr uint8_t mapWidth = (WIDTH / 8);
constexpr uint8_t mapHeight = (HEIGHT / 8);

uint8_t mapBuffer[8][16]
{
	{ 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
	{ 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01 },
	{ 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01 },
	{ 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01 },
	{ 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }, 
	{ 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01 },
	{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01 }, 
	{ 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
};

// Low nibbles left half wall tile
// High nibbles right half wall tile
const uint8_t PROGMEM wallDecodeTable[] =
{
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0xA6, 0xB7,
	0x00, 0x11, 0x22, 0x88, 0x44, 0x55, 0xA6, 0xC9,
};


Arduboy2Base arduboy;
uint8_t borderTile; 
Point cursor;
bool cursorChanged;
uint8_t cursorFrame;

void setup()
{
	arduboy.boot(); //minimal boot
}

uint8_t getTile(int x, int y)
{
	if ((x < 0) || (x >= WIDTH) || (y < 0) || (y >= HEIGHT))
		return borderTile;

	return mapBuffer[y / 8][x / 8];
}

void drawWalls()
{
	for (int y = 0; y < HEIGHT; y += 8)
		for (int x = 0; x < WIDTH; x += 8)
		{
			//// draw outlined wall tiles ////
			if (getTile(x,y) != WALL)
				continue;

			uint8_t wall = 0;

			if (getTile(x + 0, y - 8) == WALL)
				wall |= 0x01;

			if (getTile(x - 8, y + 0) == WALL)
				wall |= 0x02;

			if (getTile(x + 0, y + 8) == WALL)
				wall |= 0x04;

			if (getTile(x - 8, y - 8) == WALL)
				wall |= 0x08;

			uint8_t tile = pgm_read_byte(&wallDecodeTable[wall]);

			if (getTile(x - 8, y + 8) == WALL) tile = tile / 16;
				tile &= 0x0F;

			Sprites::drawSelfMasked(x, y, leftWallTiles, tile);

			// Clear left walls
			wall &= 0x05;

			if (getTile(x + 8, y + 0) == WALL)
				wall |= 0x02;

			if (getTile(x + 8, y - 8) == WALL)
				wall |= 0x08;

			tile = pgm_read_byte(&wallDecodeTable[wall]);

			if (getTile(x + 8, y + 8) == WALL)
				tile /= 16;

			tile &= 0x0F;

			Sprites::drawSelfMasked(x + 4, y, rightWallTiles, tile);
		}
}

void loop()
{
	// Wait for start of frame
	if (!arduboy.nextFrame())
		return;
		
	arduboy.pollButtons();
		
	// 4 frames
	if (arduboy.everyXFrames(7))
		cursorFrame = ((cursorFrame + 1) % 4);
	
	cursorChanged = false;
	
	if (arduboy.justPressed(UP_BUTTON))
	{
		--cursor.y;
		cursorChanged = true;
	}
	
	if (arduboy.justPressed(DOWN_BUTTON))
	{
		++cursor.y;
		cursorChanged = true;
	}
	
	if (arduboy.justPressed(LEFT_BUTTON))
	{
		--cursor.x;
		cursorChanged = true;
	}
	
	if (arduboy.justPressed(RIGHT_BUTTON))
	{
		++cursor.x;
		cursorChanged = true;
	}
	
	//keep y in range 0..7
	cursor.y %= 8;
	
	//keep x in range 0..15
	cursor.x %= 16;
	
	if (arduboy.justPressed(A_BUTTON) || (arduboy.pressed(A_BUTTON) && cursorChanged))
		mapBuffer[cursor.y][cursor.x] ^= WALL;
		
	if (arduboy.justPressed(B_BUTTON))
		borderTile ^= WALL;

	drawWalls();
	
	Sprites::drawPlusMask(cursor.x * 8, cursor.y * 8, cursorSprite, cursorFrame);
	
	arduboy.display(CLEAR_BUFFER);
}

