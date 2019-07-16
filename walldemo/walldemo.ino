// auto wall outline demo by Mr.Blinky July 2019

// use D-Pad to move cursors 
// A button to toggle Wall / no-wall tile
// B button to toggle wall / no-wall border 

#include <Arduboy2.h>
#include "Images.h"
#include "Walls.h"
#include "Tile.h"
#include "WallDecode.h"

Arduboy2Base arduboy;
Tile borderTile = Tile::Empty;
Point cursor;
bool cursorChanged;
uint8_t cursorFrame;

constexpr uint8_t tileWidth = 8;
constexpr uint8_t tileHeight = 8;

constexpr uint8_t mapWidth = (WIDTH / tileWidth);
constexpr uint8_t mapHeight = (HEIGHT / tileHeight);

Tile mapBuffer[mapHeight][mapWidth]
{
	{ Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall },
	{ Tile::Wall, Tile::Wall, Tile::Wall, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Wall, Tile::Wall, Tile::Empty, Tile::Wall, Tile::Empty, Tile::Wall },
	{ Tile::Wall, Tile::Wall, Tile::Wall, Tile::Empty, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Empty, Tile::Wall, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Wall },
	{ Tile::Wall, Tile::Empty, Tile::Empty, Tile::Wall, Tile::Wall, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Wall, Tile::Empty, Tile::Wall, Tile::Empty, Tile::Wall, Tile::Wall },
	{ Tile::Wall, Tile::Empty, Tile::Wall, Tile::Empty, Tile::Wall, Tile::Wall, Tile::Empty, Tile::Wall, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Wall }, 
	{ Tile::Wall, Tile::Empty, Tile::Wall, Tile::Empty, Tile::Wall, Tile::Wall, Tile::Empty, Tile::Wall, Tile::Empty, Tile::Wall, Tile::Wall, Tile::Empty, Tile::Empty, Tile::Wall, Tile::Wall, Tile::Wall },
	{ Tile::Wall, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Wall, Tile::Empty, Tile::Wall, Tile::Empty, Tile::Wall }, 
	{ Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall, Tile::Wall },
};

inline Tile getTile(int8_t x, int8_t y)
{
	return ((x < 0) || (x >= mapWidth) || (y < 0) || (y >= mapHeight)) ? borderTile : mapBuffer[y][x];
}

void handleInput()
{
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
	
	// Keep y in range 0..7
	cursor.y %= mapHeight;
	
	// Keep x in range 0..15
	cursor.x %= mapWidth;
	
	if (arduboy.justPressed(A_BUTTON) || (arduboy.pressed(A_BUTTON) && cursorChanged))
		toggle(mapBuffer[cursor.y][cursor.x]);
		
	if (arduboy.justPressed(B_BUTTON))
		toggle(borderTile);
}

void drawWalls()
{
	for (int8_t y = 0; y < mapHeight; ++y)
		for (int8_t x = 0; x < mapWidth; ++x)
		{
			if (getTile(x, y) != Tile::Wall)
				continue;
				
			const uint8_t drawX = (x * tileWidth);
			const uint8_t drawY = (y * tileHeight);

			Walls verticalWalls = Walls::None;

			if (getTile(x, y - 1) == Tile::Wall)
				verticalWalls |= Walls::Above;

			if (getTile(x, y + 1) == Tile::Wall)
				verticalWalls |= Walls::Below;

			// Draw left tile
			{
				Walls leftWalls = verticalWalls;
			
				if (getTile(x - 1, y) == Tile::Wall)
					leftWalls |= Walls::Side;

				if (getTile(x - 1, y - 1) == Tile::Wall)
					leftWalls |= Walls::Corner;
				
				const uint8_t decoded = wallDecode(leftWalls);
				const uint8_t tileIndex = (getTile(x - 1, y + 1) == Tile::Wall) ? ((decoded >> 4) & 0x0F) : ((decoded >> 0) & 0x0F);
				Sprites::drawSelfMasked(drawX, drawY, leftWallTiles, tileIndex);
			}

			// Draw right tile
			{
				Walls rightWalls = verticalWalls;
				
				if (getTile(x + 1, y) == Tile::Wall)
					rightWalls |= Walls::Side;

				if (getTile(x + 1, y - 1) == Tile::Wall)
					rightWalls |= Walls::Corner;

				const uint8_t decoded = wallDecode(rightWalls);
				const uint8_t tileIndex = (getTile(x + 1, y + 1) == Tile::Wall) ? ((decoded >> 4) & 0x0F) : ((decoded >> 0) & 0x0F);
				Sprites::drawSelfMasked(drawX + 4, drawY, rightWallTiles, tileIndex);
			}
		}
}

void setup()
{
	arduboy.boot(); //minimal boot
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
	
	handleInput();

	drawWalls();
	
	Sprites::drawPlusMask((cursor.x * tileWidth), (cursor.y * tileHeight), cursorSprite, cursorFrame);
	
	arduboy.display(CLEAR_BUFFER);
}

