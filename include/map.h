#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include <raylib.h>
#include "game.h"

typedef enum{
  TILE_AIR,
  TILE_BLOCK,
  TILE_WALL,
  TILE_DEADLY
} TileType;

typedef struct{
  TileType tileType;
  Vector2 position;
  Rectangle textureRec;
} Tile;

typedef struct{
  Tile *tiles;
  bool complete;
  const char* mapName;
  Vector2 playerPosition;
} TileMap;

typedef struct{
  TileMap *tileMaps;
  int quantity;
} Maps;

bool InitMaps(Maps *maps, uint32_t (*Map)[MAPWIDTH*MAPHEIGHT]);
void DrawMaps(TileMap *tileMap);
void FreeMaps(Maps *maps);

#endif
