// auto wall outline demo by Mr.Blinky July 2019

// use D-Pad to move cursors 
// A button to toggle Wall / no-wall tile
// B button to toggle wall / no-wall border 

#include <Arduboy2.h>
#include "leftwalltiles.h"
#include "rightwalltiles.h"
#include "cursorsprite.h"
#include "map.h"
#include "tables.h"


#define WALL 0x01
#define MAP_WIDTH (WIDTH / 8)
#define MAP_HEIGHT (HEIGHT / 8)

Arduboy2Base arduboy;
uint8_t borderTile; 
Point cursor;
uint8_t cursor_frame;

void setup()
{
  arduboy.boot(); //minimal boot
}

uint8_t getTile(int x, int y)
{
  if ((x < 0) || (x >= WIDTH) || (y < 0) || (y >= HEIGHT)) return borderTile;
  return mapBuffer[y / 8][x / 8];
}

void  drawWalls()
{
  for (int y = 0; y < HEIGHT; y += 8)
    for (int x = 0; x < WIDTH; x += 8)
    {
     //// draw outlined wall tiles ////
     if (getTile(x,y) == WALL)
     {
       uint8_t wall=0;
       if (getTile(x + 0, y - 8) == WALL) wall |= 0x01;
       if (getTile(x - 8, y + 0) == WALL) wall |= 0x02;
       if (getTile(x + 0, y + 8) == WALL) wall |= 0x04;
       if (getTile(x - 8, y - 8) == WALL) wall |= 0x08;
       uint8_t tile = pgm_read_byte(&wallDecodeTable[wall]);
       if (getTile(x - 8, y + 8) == WALL) tile = tile / 16;
       tile &= 0x0F;
       Sprites::drawSelfMasked(x, y, leftwalltiles, tile);
       wall &= 0x05; //clear left walls
       if (getTile(x + 8, y + 0) == WALL) wall |= 0x02;
       if (getTile(x + 8, y - 8) == WALL) wall |= 0x08;
       tile = pgm_read_byte(&wallDecodeTable[wall]);
       if (getTile(x + 8, y + 8) == WALL) tile = tile / 16;
       tile &= 0x0F;
       Sprites::drawSelfMasked(x+4, y, rightwalltiles, tile);
     }    
    }
}

void loop()
{
  if (!arduboy.nextFrame()) return; //wait for start of frame
  if (arduboy.everyXFrames(7)) cursor_frame = (cursor_frame + 1) & 3;// 4 frames
  arduboy.pollButtons();
  if (arduboy.justPressed(UP_BUTTON)) cursor.y--;
  if (arduboy.justPressed(DOWN_BUTTON)) cursor.y++;
  if (arduboy.justPressed(LEFT_BUTTON)) cursor.x--;
  if (arduboy.justPressed(RIGHT_BUTTON)) cursor.x++;
  cursor.y &= 0x07; //keep y in range 0..7
  cursor.x &= 0x0F; //keep x in range 0..15
  if (arduboy.justPressed(A_BUTTON)) mapBuffer[cursor.y][cursor.x] ^= WALL;
  if (arduboy.justPressed(B_BUTTON)) borderTile ^= WALL;
  drawWalls();
  Sprites::drawPlusMask(cursor.x * 8, cursor.y * 8, cursorsprite, cursor_frame);
  arduboy.display(CLEAR_BUFFER);  
}

