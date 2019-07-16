// auto wall outline demo by Mr.Blinky July 2019

// use D-Pad to move cursors 
// A button to toggle Wall / no-wall tile
// B button to toggle wall / no-wall border 

#include <Arduboy2.h>
#include "Images.h"
#include "map.h"
#include "tables.h"


#define WALL 0x01
#define MAP_WIDTH (WIDTH / 8)
#define MAP_HEIGHT (HEIGHT / 8)

Arduboy2Base arduboy;
uint8_t borderTile; 
Point cursor;
bool cursor_changed;
uint8_t cursor_frame;

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

			Sprites::drawSelfMasked(x, y, leftwalltiles, tile);

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

			Sprites::drawSelfMasked(x + 4, y, rightwalltiles, tile);
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
		cursor_frame = ((cursor_frame + 1) % 4);
	
	cursor_changed = false;
	
	if (arduboy.justPressed(UP_BUTTON))
	{
		--cursor.y;
		cursor_changed = true;
	}
	
	if (arduboy.justPressed(DOWN_BUTTON))
	{
		++cursor.y;
		cursor_changed = true;
	}
	
	if (arduboy.justPressed(LEFT_BUTTON))
	{
		--cursor.x;
		cursor_changed = true;
	}
	
	if (arduboy.justPressed(RIGHT_BUTTON))
	{
		++cursor.x;
		cursor_changed = true;
	}
	
	//keep y in range 0..7
	cursor.y %= 8;
	
	//keep x in range 0..15
	cursor.x %= 16;
	
	if (arduboy.justPressed(A_BUTTON) || (arduboy.pressed(A_BUTTON) && cursor_changed))
		mapBuffer[cursor.y][cursor.x] ^= WALL;
		
	if (arduboy.justPressed(B_BUTTON))
		borderTile ^= WALL;

	drawWalls();
	
	Sprites::drawPlusMask(cursor.x * 8, cursor.y * 8, cursorsprite, cursor_frame);
	
	arduboy.display(CLEAR_BUFFER);
}

