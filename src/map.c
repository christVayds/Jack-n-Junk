#include <stdlib.h>
#include "map.h"

#include <stdio.h>

bool InitMaps(Maps *maps, uint32_t (*Map)[MAPWIDTH*MAPHEIGHT]){
  maps->tileMaps = (TileMap*)malloc(sizeof(TileMap)*MAPCOUNT);
  if(!maps->tileMaps) return false;

  printf("print maps\n");
  for(uint32_t i=0;i<MAPCOUNT;i++){
    TileMap tileMap = {0};
    tileMap.tiles = (Tile*)malloc(sizeof(Tile) * MAPWIDTH * MAPHEIGHT);
    if(!tileMap.tiles) return false;

    tileMap.complete = false;
    tileMap.mapName = "Map";
    
    Vector2 position = {0,0};
    uint32_t countWidth = 0;
    for(uint32_t j=0;j<MAPWIDTH*MAPHEIGHT;j++){
      Tile tile = {0};
      tile.position = position;
      
      switch(Map[i][j]){
        case 0:{  // AIR 
          tile.tileType = TILE_AIR;
          break;
        }
        case 1:{  // BLOCK
          tile.tileType = TILE_BLOCK;
          tile.textureRec = (Rectangle){0, 0, TILESIZE, TILESIZE};
          break;
        }
        case 10:{ // PLAYER POSITION 
          tile.tileType = TILE_AIR;
          tileMap.playerPosition.x = position.x;
          tileMap.playerPosition.y = position.y - PLAYER_HEIGHT;
          break;
        }
        default:{ // DEFAULT
          tile.tileType = TILE_AIR;
          break;
        }
      }

      // push tile to tileMaps
      tileMap.tiles[j] = tile;
      
      // modify position
      position.x += TILESIZE;
      countWidth++;
      if(countWidth >= MAPWIDTH){
        position.x = 0;
        position.y += TILESIZE;
        countWidth = 0;
      }
    }
    maps->tileMaps[i] = tileMap;
  };

  return true;
}

void DrawMaps(TileMap *tileMap){
  for(uint32_t y=0;y<MAPHEIGHT;y++){
    for(uint32_t x=0;x<MAPWIDTH;x++){
      const uint32_t index = y * MAPWIDTH + x;
      if(tileMap->tiles[index].tileType){
        Rectangle rec = (Rectangle){tileMap->tiles[index].position.x, tileMap->tiles[index].position.y, TILESIZE, TILESIZE};
        DrawRectangleRec(rec, WHITE);
      }
    }
  }
}

void FreeMaps(Maps *maps){
  for(uint32_t i=0;i<MAPCOUNT;i++){
    free(maps->tileMaps[i].tiles);
  }
  free(maps->tileMaps);
}
