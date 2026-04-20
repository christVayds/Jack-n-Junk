#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <raylib.h>

#define SCREENWIDTH 800
#define SCREENHEIGHT 500
#define MAXFPS 60

// PHYSICS
#define GRAVITY 800.0f

// TILE MAP 
#define TILESIZE 32
#define MAPCOUNT 1
#define MAPWIDTH 10
#define MAPHEIGHT 5

// BUILD MODE
#define DEBUG_MODE true

// ENTITY
#define PLAYER_WIDTH 48
#define PLAYER_HEIGHT 48

typedef struct{
  int32_t startX, startY, endX, endY;
} WorldPoints;

WorldPoints BlocksVisibleInCamera(Camera2D *camera);

#endif
