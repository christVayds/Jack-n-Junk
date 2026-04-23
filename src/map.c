#include <stdlib.h>
#include "map.h"

static void AnimateTile(Tile *tile){
  tile->frameCounter++;
  if(tile->frameCounter >= (MAXFPS / FRAMESPEED)){
    tile->frameCounter = 0;
    tile->currentFrame++;
    
    // frame end 
    if(tile->currentFrame > 3)
      tile->currentFrame = 0;
    tile->textureRec.x = tile->animX + tile->currentFrame * tile->textureRec.width;
  }
}

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
      tile.currentFrame = 0;
      tile.frameCounter = 0;
      tile.depth = 0;
      tile.text = NULL;
      tile.canCollide = true;
      tile.animX = 0;
      
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
          tile.textureRec = (Rectangle){0, 0, TILESIZE, TILESIZE};
          break;
        }
        case 12:{ // MOVING VERTICAL TILES 
          tile.tileType = TILE_MOVE;
          tile.textureRec = (Rectangle){96, 64, 64, 32};
          tile.velocity = (Vector2){240, 0};
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
        case 15:{ // HIDDING SPIKE TILES  / TO BE REMOVED
          tile.tileType = TILE_DEADLY;
          tile.textureRec = (Rectangle){96, 0, TILESIZE, TILESIZE};
          tile.visible = false;
          break;
        }
        // SPRINT TILE RIGHT DIRECTION
        case 16:{ // SPRINT TILE LEFT SIZE 
          tile.tileType = TILE_SPRINT;
          tile.textureRec = (Rectangle){160, 0, TILESIZE, TILESIZE};
          tile.velocity = (Vector2){1, 0};
          break;
        }
        case 17:{ // SPRINT TILE MIDDLE
          tile.tileType = TILE_SPRINT;
          tile.textureRec = (Rectangle){160, 32, TILESIZE, TILESIZE};
          tile.velocity = (Vector2){1, 0};
          tile.animX = 160;
          break;
        }
        case 18:{ // SPRINT TILE RIGHT
          tile.tileType = TILE_SPRINT;
          tile.textureRec = (Rectangle){160, 64, TILESIZE, TILESIZE};
          tile.velocity = (Vector2){1, 0};
          tile.animX = 160;
          break;
        }
        // SPRINT TILE LEFT DIRECTION
        case 19:{ // SPRINT TILE LEFT SIZE 
          tile.tileType = TILE_SPRINT;
          tile.textureRec = (Rectangle){160, 0, TILESIZE, TILESIZE};
          tile.velocity = (Vector2){-1, 0};
          tile.animX = 160;
          break;
        }
        case 20:{ // SPRINT TILE MIDDLE
          tile.tileType = TILE_SPRINT;
          tile.textureRec = (Rectangle){160, 32, TILESIZE, TILESIZE};
          tile.velocity = (Vector2){-1, 0};
          tile.animX = 160;
          break;
        }
        case 21:{ // SPRINT TILE MIDDLE
          tile.tileType = TILE_SPRINT;
          tile.textureRec = (Rectangle){160, 64, TILESIZE, TILESIZE};
          tile.velocity = (Vector2){-1, 0};
          tile.animX = 160;
          break;
        }
        case 22:{  // BLOCK (TOP-LEFT BORDER)
          tile.tileType = TILE_BLOCK;
          tile.textureRec = (Rectangle){0, 96, TILESIZE, TILESIZE};
          break;
        }
        case 23:{  // BLOCK (TOP-RIGHT BORDER)
          tile.tileType = TILE_BLOCK;
          tile.textureRec = (Rectangle){0, 128, TILESIZE, TILESIZE};
          break;
        }
        case 24:{  // BLOCK (FULL BORDER)
          tile.tileType = TILE_BLOCK;
          tile.textureRec = (Rectangle){0, 160, TILESIZE, TILESIZE};
          break;
        }
        case 25:{  // BLOCK (SIZE BORDER)
          tile.tileType = TILE_BLOCK;
          tile.textureRec = (Rectangle){0, 192, TILESIZE, TILESIZE};
          break;
        }
        case 26:{  // TRASH
          tile.tileType = TILE_OTHER;
          tile.textureRec = (Rectangle){0, 224, TILESIZE, TILESIZE};
          tile.depth = 1;
          tile.canCollide = false;
          break;
        }
        case 27:{  // STICK
          tile.tileType = TILE_OTHER;
          tile.textureRec = (Rectangle){0, 256, TILESIZE, TILESIZE};
          tile.depth = 1;
          tile.canCollide = false;
          break;
        }
        case 28:{  // SMALL STICK
          tile.tileType = TILE_OTHER;
          tile.textureRec = (Rectangle){0, 288, TILESIZE, TILESIZE};
          tile.depth = 1;
          tile.canCollide = false;
          break;
        }
        case 29:{  // Jack wreak head
          tile.tileType = TILE_OTHER;
          tile.textureRec = (Rectangle){0, 320, TILESIZE, TILESIZE};
          tile.depth = 1;
          tile.canCollide = false;
          break;
        }
        case 30:{  // Jack wreak body
          tile.tileType = TILE_OTHER;
          tile.textureRec = (Rectangle){0, 352, PLAYER_WIDTH, TILESIZE};
          tile.depth = 1;
          tile.canCollide = false;
          break;
        }
        case 31:{  // Sign board
          tile.tileType = TILE_OTHER;
          tile.textureRec = (Rectangle){0, 384, 48, 48};
          tile.position.y -= tile.textureRec.height - TILESIZE;
          tile.canCollide = false;
          break;
        }
        case 32:{  // Bridge
          tile.tileType = TILE_OTHER;
          tile.textureRec = (Rectangle){0, 432, 144, 32};
          tile.canCollide = false;
          break;
        }
        case 33:{  // Charge
          tile.tileType = TILE_MACHINE;
          tile.textureRec = (Rectangle){288, 0, 48, 64};
          tile.position.y -= tile.textureRec.height - TILESIZE;
          tile.canCollide = false;
          tile.animX = 288;
          break;
        }
        case 34:{  // Push upward
          tile.tileType = TILE_MACHINE;
          tile.textureRec = (Rectangle){336, 64, TILESIZE, TILESIZE};
          tile.velocity = (Vector2){0, -1};
          break;
        }
        case 35:{  // Text 
          tile.tileType = TILE_TEXT;
          tile.text = "Get the flower and protect it.";
          tile.canCollide = false;
          break;
        }
        case 36:{ // WASD CONTROL
          tile.tileType = TILE_OTHER;
          tile.textureRec = (Rectangle){0, 528, 128, 65};
          tile.canCollide = false;
          break;
        }
        case 37:{ // Sunflower
          tile.tileType = TILE_FLOWER;
          tile.textureRec = (Rectangle){480, 0, 16, 32};
          //tile.canCollide = false;
          tile.animX = 480;
          break;
        }
        case 38:{  // Text 
          tile.tileType = TILE_TEXT;
          tile.text = "Jump here";
          tile.canCollide = false;
          break;
        }
        case 39:{  // Text 
          tile.tileType = TILE_TEXT;
          tile.text = "Find the Life Machine Before your battery died.";
          tile.canCollide = false;
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
      if(tile->visible){
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
          case TILE_SPRINT:{
            if(tile->animX)
              AnimateTile(tile);
            break;
          }
          case TILE_MACHINE:{
            if(tile->animX)
              AnimateTile(tile);
            break;
          }
          case TILE_FLOWER:{
            if(tile->animX)
              AnimateTile(tile);
          }
        }
      }
    }
  }
}


// TODO: ADD WORLDPOINTS
void DrawMaps(TileMap *tileMap, Texture2D texture, Font font, const uint32_t depth){
  for(uint32_t y=0;y<MAPHEIGHT;y++){
    for(uint32_t x=0;x<MAPWIDTH;x++){
      const uint32_t index = y * MAPWIDTH + x;
      Tile *tile = &tileMap->tiles[index];
      if(tile->tileType && tile->visible && tile->depth == depth){
        //Rectangle rec = (Rectangle){tileMap->tiles[index].position.x, tileMap->tiles[index].position.y, TILESIZE, TILESIZE}; 
        //DrawRectangleRec(rec, WHITE);
        
        // DRAW TEXT 
        if(tile->tileType == TILE_TEXT)
          DrawTextEx(font, tile->text, tile->position, 16.0f, 1, WHITE);
        else // DRAW TEXTURE
          DrawTextureRec(texture, tile->textureRec, tile->position, WHITE);
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
  for(int32_t y=0;y<MAPHEIGHT;y++){
    for(int32_t x=0;x<MAPWIDTH;x++){
      int32_t index = y * MAPWIDTH + x;
      Tile *tile = &tileMap->tiles[index];
      
      Rectangle tileRec = {tile->position.x, tile->position.y, tile->textureRec.width, tile->textureRec.height};
      tile->playerEnter = false;
      
      if(CheckCollisionRecs(entityRec, tileRec) && tile->tileType && tile->canCollide){
        if(tile->tileType == TILE_FLOWER){
          tile->tileType = TILE_AIR;
          return NULL;
        }
        tile->playerEnter = true;
        tile->playerEntered = true;
        return tile;
      }
    }
  }

  return NULL;
}
