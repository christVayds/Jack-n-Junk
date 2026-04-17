#include "entity.h"
#include "game.h"
#include <stdlib.h>

Entity EntityNew(Vector2 position, Rectangle textureRec, int32_t life){
  Entity newEntity = {
    .position = position,
    .textureRec = textureRec,
    .life = life,
    .acceleration = 600.0f,
    .velocity = {0},
    .isGround = false,
    .maxSpeed = 300.0f,
    .friction = 800.0f,
    .jumpForce = -400.0f
  };

  return newEntity;
}

void EntitySetPosition(Entity *entity, Vector2 position){
  entity->position.x = position.x;
  entity->position.y = position.y;
}

Tile *GetTileCollide(Rectangle entityRec, TileMap *tileMap, WorldPoints *worlPoints){
  Tile *tileIndex = NULL;
  for(int32_t y=worlPoints->startY;y<worlPoints->endY;y++){
    for(int32_t x=worlPoints->startX;x<worlPoints->endX;x++){
      int32_t index = y * MAPWIDTH + x;
      Rectangle tileRec = {tileMap->tiles[index].position.x, tileMap->tiles[index].position.y, TILESIZE, TILESIZE};
      if(CheckCollisionRecs(entityRec, tileRec) && tileMap->tiles[index].tileType){
        tileIndex = &tileMap->tiles[index];
      }
    }
  }

  return tileIndex;
}

void EntityMove(Entity *entity, const float dt, TileMap *tileMap, Camera2D *camera){
  Vector2 move = {0};
  if(IsKeyDown(KEY_D)) move.x = 1;
  if(IsKeyDown(KEY_A)) move.x = -1;

  // JUMP
  if(IsKeyPressed(KEY_SPACE) && entity->isGround){
    entity->velocity.y = entity->jumpForce;
    entity->isGround = false;
  }

  // update velocity
  entity->velocity.x += entity->acceleration * move.x * dt;
  entity->velocity.y += GRAVITY * dt;

  if(!IsKeyDown(KEY_D) && !IsKeyDown(KEY_A)){
    if(entity->velocity.x > 0){
      entity->velocity.x -= entity->friction * dt;
      if(entity->velocity.x < 0) entity->velocity.x = 0;
    } else if(entity->velocity.x < 0){
      entity->velocity.x += entity->friction * dt;
      if(entity->velocity.x > 0) entity->velocity.x = 0;
    }
  }

  // clamp speed 
  if(entity->velocity.x > entity->maxSpeed) entity->velocity.x = entity->maxSpeed;
  if(entity->velocity.x < -entity->maxSpeed) entity->velocity.x = -entity->maxSpeed;

  Rectangle desiredPosition = {.x=entity->position.x, .y=entity->position.y, .width=PLAYER_WIDTH, .height=PLAYER_HEIGHT};
  desiredPosition.x += entity->velocity.x * dt;

  Tile *tileX = NULL;
  Tile *tileY = NULL;

  WorldPoints points = BlocksVisibleInCamera(camera);

  // TODO: check x collision
  tileX = GetTileCollide(desiredPosition, tileMap, &points);

  // apply player's x position 
  if(!tileX){
    entity->position.x = desiredPosition.x;
  } else {
    // snap player position 
    if(entity->velocity.x > 0)
      entity->position.x = tileX->position.x - PLAYER_WIDTH;
    else if(entity->velocity.x < 0)
      entity->position.x = tileX->position.x + TILESIZE;
    entity->velocity.x = 0;
  }

  // update player's y position 
  desiredPosition.x = entity->position.x;
  desiredPosition.y = entity->position.y + entity->velocity.y * dt;

  // TODO: check player's y collision
  tileY = GetTileCollide(desiredPosition, tileMap, &points);

  // upply player's y position 
  entity->isGround = false;
  if(!tileY){
    entity->position.y = desiredPosition.y;
  } else {
    // snap player position
    if(entity->velocity.y > 0){
      entity->position.y = tileY->position.y - PLAYER_HEIGHT;
      entity->isGround = true;
    } else if(entity->velocity.y < 0){
      entity->position.y = tileY->position.y + TILESIZE;
    }
    entity->velocity.y = 0;
  } 
}

void EntityDraw(Entity *entity){
  DrawRectangleRec((Rectangle){entity->position.x, entity->position.y, PLAYER_WIDTH, PLAYER_HEIGHT}, RED);
}
