#include <stdlib.h>
#include "map.h"

bool InitMaps(Maps *maps, uint32_t (*Map)[MAPWIDTH*MAPHEIGHT]){
  maps->tileMaps = (TileMap*)malloc(sizeof(TileMap)*MAPCOUNT);
  if(!maps->tileMaps) return false;

  for(uint32_t i=0;i<MAPCOUNT;i++){
    // SET TILEMAP
    TileMap tileMap = {0};
    tileMap.tiles = (Tile*)malloc(sizeof(Tile) * MAPWIDTH * MAPHEIGHT);
    if(!tileMap.tiles) return false;

    tileMap.complete = false;
    tileMap.mapName = "Map";
    
    Vector2 position = {0,0};
    uint32_t countWidth = 0;
    for(uint32_t j=0;j<MAPWIDTH*MAPHEIGHT;j++){
      // SET TILE 
      Tile tile = {0};
      tile.position = position;
      tile.velocity = (Vector2){0};
      tile.move = false;
      tile.playerEnter = false;
      tile.playerEntered = false;
      tile.visible = true;
      
      switch(Map[i][j]){
        case 0:{  // AIR 
          tile.tileType = TILE_AIR;
          break;
        }
        case 1:{  // BLOCK (TOP)
          tile.tileType = TILE_BLOCK;
          tile.textureRec = (Rectangle){32, 0, TILESIZE, TILESIZE};
          break;
        }
        case 2:{  // BLOCK (TOP-LEFT)
          tile.tileType = TILE_BLOCK;
          tile.textureRec = (Rectangle){0, 0, TILESIZE, TILESIZE};
          break;
        }
        case 3:{  // BLOCK (TOP-RIGHT)
          tile.tileType = TILE_BLOCK;
          tile.textureRec = (Rectangle){64, 0, TILESIZE, TILESIZE};
          break;
        }
        case 4:{  // BLOCK (LEFT)
          tile.tileType = TILE_BLOCK;
          tile.textureRec = (Rectangle){0, 32, TILESIZE, TILESIZE};
          break;
        }
        case 5:{  // BLOCK (RIGHT)
          tile.tileType = TILE_BLOCK;
          tile.textureRec = (Rectangle){64, 32, TILESIZE, TILESIZE};
          break;
        }
        case 6:{  // BLOCK (BOTTOM-LEFT)
          tile.tileType = TILE_BLOCK;
          tile.textureRec = (Rectangle){0, 64, TILESIZE, TILESIZE};
          break;
        }
        case 7:{  // BLOCK (BOTTOM)
          tile.tileType = TILE_BLOCK;
          tile.textureRec = (Rectangle){32, 64, TILESIZE, TILESIZE};
          break;
        }
        case 8:{  // BLOCK (BOTTOM-RIGHT)
          tile.tileType = TILE_BLOCK;
          tile.textureRec = (Rectangle){64, 64, TILESIZE, TILESIZE};
          break;
        }
        case 9:{  // BLOCK (CENTER)
          tile.tileType = TILE_BLOCK;
          tile.textureRec = (Rectangle){32, 32, TILESIZE, TILESIZE};
          break;
        }
        case 10:{ // PLAYER POSITION 
          tile.tileType = TILE_AIR;
          tileMap.playerPosition.x = position.x;
          tileMap.playerPosition.y = position.y - PLAYER_HEIGHT;
          break;
        }
        case 11:{ // INVISIBLE BLOCK
          tile.tileType = TILE_BORDER;
          tile.visible = false;
          break;
        }
        case 12:{ // MOVING VERTICAL TILES  TODO: FIX THIS 
          tile.tileType = TILE_MOVE;
          tile.textureRec = (Rectangle){32, 32, TILESIZE, TILESIZE};
          tile.velocity = (Vector2){64, 0};
          break;
        }
        case 13:{ // MOVING HORIZONTAL TILES TODO: FIX THIS
          tile.tileType = TILE_MOVE;
          tile.textureRec = (Rectangle){32, 32, TILESIZE, TILESIZE};
          tile.velocity = (Vector2){0, -TILESIZE};
          break;
        }
        case 14:{ // SPIKE TILES 
          tile.tileType = TILE_DEADLY;
          tile.textureRec = (Rectangle){96, 0, TILESIZE, TILESIZE};
          break;
        }
        case 15:{ // HIDDING SPIKE TILES 
          tile.tileType = TILE_DEADLY;
          tile.textureRec = (Rectangle){96, 0, TILESIZE, TILESIZE};
          tile.visible = false;
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

// TODO: ADD WORLDPOINTS
void UpdateMaps(TileMap *tileMap, const float dt){
  for(uint32_t y=0;y<MAPHEIGHT;y++){
    for(uint32_t x=0;x<MAPWIDTH;x++){
      const uint32_t index = y * MAPWIDTH + x;
      Tile *tile = &tileMap->tiles[index]; 
      if(tile->visible)
        switch(tile->tileType){
          default:
            break;
          case TILE_MOVE:{
            if(tile->playerEnter && tile->playerEntered){
              tile->move = true;
              tile->position.x += tile->velocity.x * dt;
              tile->position.y += tile->velocity.y * dt;
            } else {
              tile->move = false;
            }
            break;
          }
      }
    }
  }
}


// TODO: ADD WORLDPOINTS
void DrawMaps(TileMap *tileMap, Texture2D texture){
  for(uint32_t y=0;y<MAPHEIGHT;y++){
    for(uint32_t x=0;x<MAPWIDTH;x++){
      const uint32_t index = y * MAPWIDTH + x;
      if(tileMap->tiles[index].tileType && tileMap->tiles[index].visible){
        //Rectangle rec = (Rectangle){tileMap->tiles[index].position.x, tileMap->tiles[index].position.y, TILESIZE, TILESIZE}; 
        //DrawRectangleRec(rec, WHITE);
        DrawTextureRec(texture, tileMap->tiles[index].textureRec, tileMap->tiles[index].position, WHITE);
      }
    }
  }
}

void FreeMaps(Maps *maps){
  for(uint32_t i=0;i<MAPCOUNT;i++){
    free(maps->tileMaps[i].tiles);
    maps->tileMaps[i].tiles = NULL;
  }
  free(maps->tileMaps);
  maps->tileMaps = NULL;
}

Tile *GetTileCollide(Rectangle entityRec, TileMap *tileMap, WorldPoints *worlPoints){
  Tile *tileIndex = NULL;
  for(int32_t y=0;y<MAPHEIGHT;y++){
    for(int32_t x=0;x<MAPWIDTH;x++){
      int32_t index = y * MAPWIDTH + x;
      Rectangle tileRec = {tileMap->tiles[index].position.x, tileMap->tiles[index].position.y, TILESIZE, TILESIZE};
      tileMap->tiles[index].playerEnter = false;
      
      if(CheckCollisionRecs(entityRec, tileRec) && tileMap->tiles[index].tileType){
        tileIndex = &tileMap->tiles[index];
        tileMap->tiles[index].playerEnter = true;
        tileMap->tiles[index].playerEntered = true;
      }
    }
  }

  return tileIndex;
}
