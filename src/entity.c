#include "entity.h"
#include "game.h"
#include <stdlib.h>
#include <stdio.h>

Entity EntityNew(Vector2 position){
  Entity newEntity = {
    .position = position,
    .textureRec = (Rectangle){33, 112, PLAYER_WIDTH, PLAYER_HEIGHT},
    .life = 3,
    .isAlive = true,
    .acceleration = 600.0f,
    .velocity = {0},
    .isGround = false,
    .maxSpeed = 300.0f,
    .friction = 800.0f,
    .jumpForce = -400.0f,
    .isOnPlatform = false
  };

  return newEntity;
}

void EntitySetPosition(Entity *entity, Vector2 position){
  entity->position.x = position.x;
  entity->position.y = position.y;
}

void EntityMove(Entity *entity, const float dt, TileMap *tileMap, Camera2D *camera){
  Vector2 move = {0};
  
  // check if player is alive
  if(entity->isAlive){
    if(IsKeyDown(KEY_D)) move.x = 1;
    if(IsKeyDown(KEY_A)) move.x = -1;

    // JUMP
    if(IsKeyPressed(KEY_SPACE) && entity->isGround){
      entity->velocity.y = entity->jumpForce;
      entity->isGround = false;
    }
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
  if(entity->isAlive)
    tileX = GetTileCollide(desiredPosition, tileMap, &points);


  // apply player's x position
  if(!tileX){
    entity->position.x = desiredPosition.x;
  } else {
    
    // check if not visible 
    if(!tileX->visible){
      tileX->visible = true;
    }

    switch(tileX->tileType){
      case TILE_MOVE:{
        if(tileX->move)
          entity->position.x += tileX->velocity.x * dt;
        entity->velocity.x = 0;
        break;
      }
      case TILE_DEADLY:{
        entity->isAlive = false;
        break;
      }
      default:{
        // snap player position
        if(entity->velocity.x > 0)
          entity->position.x = tileX->position.x - PLAYER_WIDTH;
        else if(entity->velocity.x < 0)
          entity->position.x = tileX->position.x + TILESIZE;
        entity->velocity.x = 0;
      }
    }   
  }

  // update player's y position 
  desiredPosition.x = entity->position.x;
  desiredPosition.y = entity->position.y + entity->velocity.y * dt;

  // TODO: check player's y collision
  if(entity->isAlive)
  tileY = GetTileCollide(desiredPosition, tileMap, &points);

  // upply player's y position 
  entity->isGround = false;
  if(!tileY){
    entity->position.y = desiredPosition.y;
  } else {
    // check if not visible
    if(!tileY->visible){
      tileY->visible = true;
    }

    switch(tileY->tileType){
      case TILE_MOVE:{
        if(tileY->move){
          entity->position.x += tileY->velocity.x * dt;
          entity->position.y += tileY->velocity.y * dt; 
        }
        
        entity->isGround = true;
        break;
      }
      case TILE_DEADLY:{
        entity->isAlive = false;
        break;
      }
      default:{
        // snap player position
        if(entity->velocity.y > 0){
          entity->position.y = tileY->position.y - PLAYER_HEIGHT;
          entity->isGround = true;
        } else if(entity->velocity.y < 0){
          entity->position.y = tileY->position.y + TILESIZE;
          entity->isGround = true;
        }
      }
      entity->velocity.y = 0;
    } 
  }
}

void EntityDraw(Entity *entity, Texture2D texture){
  //DrawRectangleRec((Rectangle){entity->position.x, entity->position.y, PLAYER_WIDTH, PLAYER_HEIGHT}, RED);
  DrawTextureRec(texture, entity->textureRec, entity->position, WHITE);
}
