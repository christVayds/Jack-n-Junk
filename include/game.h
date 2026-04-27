#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <raylib.h>

#define MAXFPS 60
#define BACKGROUNDWIDTH 640
#define BACKGROUNDHEIGHT 360

// PHYSICS
#define GRAVITY 800.0f

// TILE MAP 
#define TILESIZE 32
#define MAPCOUNT 3
#define MAPWIDTH 40
#define MAPHEIGHT 20

// BUILD MODE
#define DEBUG_MODE false

// ENTITY
#define PLAYER_WIDTH 48
#define PLAYER_HEIGHT 48

// RECT 
#define YANJILOGO (Rectangle){0, 672, 225, 225}

// ANIMATION
#define FRAMESPEED 8

#define SOUNDCOUNT 3
#define BACKGROUNDCOUNT 2

typedef struct{
  int32_t startX, startY, endX, endY;
} WorldPoints;

WorldPoints BlocksVisibleInCamera(Camera2D *camera);

#endif
