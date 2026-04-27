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
  TILE_MOVE,
  TILE_SPRINT,
  TILE_OTHER,
  TILE_MACHINE,
  TILE_MACHINEPLATFORM,
  TILE_TEXT,
  TILE_FLOWER,
  TILE_SIGNAL,
  TILE_NEXTMAP
} TileType;

typedef struct Tile Tile;

struct Tile{
  TileType tileType;
  TileType originalType;
  Vector2 position;
  Rectangle textureRec;
  Vector2 velocity;
  bool move;
  float moveLimit;      // how far the move tile moves
  float moveDistance;
  bool playerEntered;
  bool visible;
  int32_t currentFrame;
  int32_t frameCounter;
  uint32_t depth;
  char* text;
  bool canCollide;
  int32_t animX;    // x position of the first frame from the texture
  Tile *connectedTile;
};

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
void DrawMaps(TileMap *tileMap, Texture2D texture, Font font, const uint32_t depth, WorldPoints *points);
void FreeMaps(Maps *maps);
Tile *GetTileCollide(Rectangle entityRec, TileMap *tileMap, WorldPoints *worlPoints, int32_t *playerLevel);
void ResetMap(Maps *maps);
#endif
