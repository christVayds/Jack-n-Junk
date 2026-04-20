#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include <raylib.h>
#include "game.h"

typedef enum{
  TILE_AIR,
  TILE_BLOCK,
  TILE_WALL,
  TILE_DEADLY,
  TILE_BORDER,
  TILE_MOVE
} TileType;

typedef struct{
  TileType tileType;
  Vector2 position;
  Rectangle textureRec;
  Vector2 velocity;
  bool move;
  bool playerEnter;
  bool playerEntered;
  bool visible;
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
void UpdateMaps(TileMap *tileMap, const float dt);
void DrawMaps(TileMap *tileMap, Texture2D texture);
void FreeMaps(Maps *maps);
Tile *GetTileCollide(Rectangle entityRec, TileMap *tileMap, WorldPoints *worlPoints);
#endif
